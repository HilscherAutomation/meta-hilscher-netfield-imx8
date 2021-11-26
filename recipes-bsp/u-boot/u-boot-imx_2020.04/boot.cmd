# Create a bootmenu based on boot.cfg files

# menu index count
setexpr mi 0

testaddr=0x70000000

# Check for usb devices
usb reset &&
if usb dev; then
	# FIXME: Currently, only linux /dev/sda is supported!
	usbdev_found="1"
	usbdevs="0"
	usbparts="4 3 2"
fi

# Check for sd/mmc devices.
mmc rescan &&
if mmc dev; then
	mmcdev_found="1"
	mmcdevs="0 1"
	mmcparts="4 3 2"
fi

for conf in boot.cfg aboot.cfg rboot.cfg; do
	if test ${usbdev_found} = "1"; then
		for dev in ${usbdevs}; do
			for part in ${usbparts}; do
				if load usb ${dev}:${part} ${testaddr} ${conf}; then
					env import ${testaddr}
					test -z "${description}" && description="unknown"
					test "${conf}" = "boot.cfg" && type=" "
					test "${conf}" = "aboot.cfg" && type="(ALTERNATIVE)"
					test "${conf}" = "rboot.cfg" && type="(RESCUE)"
					setenv bootmenu_${mi} usb${dev}: ${description} ${type} = "
						setenv bootargs console=${console} bootCfg=/dev/sda${part}/${conf} rootwait rw rootdelay=1 roottimeout=10 loglevel=7;
						load usb ${dev}:${part} ${loadaddr} ${kernel};
						bootm
					"
					setexpr mi ${mi} + 1
				fi
			done
		done
	fi
	if test ${mmcdev_found} = "1"; then
		for dev in ${mmcdevs}; do
			for part in ${mmcparts}; do
				if load mmc ${dev}:${part} ${testaddr} ${conf}; then
					env import ${testaddr}
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
	setenv bootmenu_${mi} FastBoot = "run fastboot"
done

bootmenu 3
