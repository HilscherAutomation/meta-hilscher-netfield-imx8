#define CONFIG_LOADADDR 0x50000000 //0x40480000
#define CONFIG_SYS_LOAD_ADDR CONFIG_LOADADDR

#define CONFIG_SYS_BOOTM_LEN 0x2000000

/* GigaBit link requires longer autonegotiation */
#define PHY_ANEG_TIMEOUT 10000

#define CONFIG_BOOTP_ID_CACHE_SIZE 10

/* Initial environment variables */
#define HILSCHER_CONFIG_EXTRA_ENV_SETTINGS \
	"loadbootscript="MMC_LOAD_BOOT_SCRIPT"\0" \
	"bootscript="RUN_BOOT_SCRIPT"\0" \
	"loadimage="LOAD_IMAGE"\0" \
	"fastboot="FASTBOOT_SCRIPT"\0" \
	"netboot="NETBOOT_SCRIPT"\0" \
	"autoload=yes\0" \
	"initrd_high=0x50000000\0" \
	"fdt_high=0x48000000\0" \
	"pxe_setup="PXEBOOT_SETUP"\0" \
	"bootcmd_pxe="PXEBOOT_COMMAND"\0" \
	"netcon_ip=192.168.253.1\0" \
	"netcon_cl=192.168.253.2\0" \
	"netcon_up="NETCON_ENABLE"\0" \
	"netcon_down="SERIALCON_ENABLE"\0" \
	"menu_last_cmd=setenv bootdelay 15; run netcon_up;\0" \
	"menum=2\0" \
	USBBOOT_COMMAND \
	"set_default_led= \0" \
	"hab_stat="GET_HAB_STATUS"\0" \

#define CONFIG_BOOTCOMMAND BOOTCOMMAND_SCRIPT
#define BOOTCOMMAND_SCRIPT \
	"run hab_stat; " \
	"run set_default_led; " \
	"run bootcmd_usb0; " \
	"if test \"${boot_mode}\" = \"1\"; then " \
		"run fastboot; " \
	"fi; " \
	"mmc dev ${mmcdev}; " \
	"if mmc rescan; then " \
		"if run loadbootscript; then " \
			"run bootscript; " \
		"else " \
			"if run loadimage; then " \
				"run mmcboot; " \
			"fi; " \
		"fi; " \
	"else " \
		"booti ${loadaddr} - ${fdt_addr}; " \
	"fi; " \
	"while true; do " \
		"run pxe_setup; " \
		"run bootcmd_pxe; " \
	"done; " \

#if defined(CONFIG_IMX_HAB)
	#define GET_HAB_STATUS "hab_status"
	/* security is enabled, so first verify (if fuses are setup) and then start image */
	#define START_IMAGE "setexpr entrypoint ${loadaddr} + 20 && setenv ivt_off 0 && hab_auth_img ${loadaddr} ${filesize} ${ivt_off} && source ${entrypoint}"
#else
	#define GET_HAB_STATUS " "
	#define START_IMAGE "source ${loadaddr}"
#endif

#define MMC_LOAD_BOOT_SCRIPT \
	"fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${script} && "START_IMAGE";"

#define USB_LOAD_BOOT_SCRIPT \
	"load usb ${0}:${part} ${loadaddr} ${script} && "START_IMAGE";"

#define RUN_BOOT_SCRIPT \
	"echo Running bootscript from mmc ...; source;" \

#define LOAD_IMAGE \
	"fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${image};" \

#define FASTBOOT_SIMPLE \
	"while true; do setenv ipaddr 192.168.253.1; fastboot udp; done;" \

#define FASTBOOT_SCRIPT \
	"echo IP: 192.168.253.1;" \
	"echo Disabling netCONSOLE...;" \
	"run netcon_down;" \
	"sleep 2;" \
	"while true; do setenv ipaddr 192.168.253.1; fastboot udp; done;" \

#define NETBOOT_SCRIPT \
	"netboot=echo Booting from net ...; " \
	"run netargs;  " \
	"if test ${ip_dyn} = yes; then " \
		"setenv get_cmd dhcp; " \
	"else " \
		"setenv get_cmd tftp; " \
	"fi; " \
	"${get_cmd} ${loadaddr} ${image}; " \
	"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
		"if ${get_cmd} ${fdt_addr} ${fdt_file}; then " \
			"booti ${loadaddr} - ${fdt_addr}; " \
		"else " \
			"echo WARN: Cannot load the DT; " \
		"fi; " \
	"else " \
		"booti; " \
	"fi; " \

#define PXEBOOT_SETUP \
	"setenv kernel_addr_r ${loadaddr};setenv ramdisk_addr_r ${loadaddr};setenv fdt_addr ${loadaddr};setenv pxefile_addr_r ${loadaddr};setenv bootargs console=$console provisioning=1;" \

#define PXEBOOT_COMMAND \
	"dhcp; " \
	"if test $? = 0; then " \
		"pxe boot; " \
	"fi; " \

#define SERIALCON_ENABLE \
	"setenv stdout serial; setenv stdin serial;" \

#define NETCON_ENABLE \
	"setenv ipaddr $netcon_ip;setenv ncip $netcon_cl;setenv stdout nc; setenv stdin nc;" \

#define USBBOOT_COMMAND  \
	"usb_parts=1\0"                                                                                  \
	"bootcmd_usb0="                                                                                  \
		"if usb reset && usb dev; then "                                                         \
			"for part in ${usb_parts}; do "                                                  \
				"if test -e usb 0:${part} ${script}; then "                              \
					"echo Found U-Boot script ${script}; "                           \
					USB_LOAD_BOOT_SCRIPT                                             \
					"echo SCRIPT FAILED: continuing...; "                            \
				"fi; "                                                                   \
			"done; "                                                                         \
		"fi;\0"
