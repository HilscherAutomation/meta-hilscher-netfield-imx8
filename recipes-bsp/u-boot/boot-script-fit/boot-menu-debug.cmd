# Create a bootmenu based on boot.cfg files

# menu index count
setexpr mi 0

# Check for sd/mmc devices.
mmc rescan &&
if mmc dev; then
	mmcdev_found="1"
	mmcdevs="0 1"
	mmcparts="4 3 2"
fi

for conf in boot.cfg aboot.cfg rboot.cfg; do
	if test ${mmcdev_found} = "1"; then
		for dev in ${mmcdevs}; do
			for part in ${mmcparts}; do
				if load mmc ${dev}:${part} ${loadaddr} ${conf}; then
					env import ${loadaddr}
					test -z "${description}" && description="unknown"
					test "${conf}" = "boot.cfg" && type=" "
					test "${conf}" = "aboot.cfg" && type="(ALTERNATIVE)"
					test "${conf}" = "rboot.cfg" && type="(RESCUE)"
					setenv bootmenu_${mi} mmc${dev}: ${description} ${type} = "
						setenv bootargs console=${console} bootCfg=/dev/mmcblk${dev}p${part}/${conf} rootwait rw rootdelay=1 roottimeout=10 loglevel=7;
						load mmc ${dev}:${part} ${loadaddr} ${kernel};
						bootm
					"
					setexpr mi ${mi} + 1
				fi
			done
		done
	fi
done

setenv bootmenu_${mi} FastBoot = "run fastboot"
setexpr mi ${mi} + 1
setenv bootmenu_${mi} Console = "run setup_console"

setexpr boot_menu_max ${mi} + 1
# set $boot_menu (default selection) variably by call to get_menu
run get_menu

if test $boot_menu -gt $mi; then
	# in case menu counter is bigger the pin control might be defect, so set to default
	setenv boot_menu 0
fi
setenv bootmenu_default $boot_menu

bootmenu 3
