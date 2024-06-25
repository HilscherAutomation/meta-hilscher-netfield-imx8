/* config of hilscher-ucm-imx8m-mini */

/* default is 4 which lead to connection trouble (dhcp/bootp) in some network setups */
#ifdef CONFIG_BOOTP_ID_CACHE_SIZE
	#undef CONFIG_BOOTP_ID_CACHE_SIZE
	#define CONFIG_BOOTP_ID_CACHE_SIZE 10
#endif

#ifdef CONFIG_LOADADDR
	#undef CONFIG_LOADADDR
	#define CONFIG_LOADADDR      0x50000000
#endif

#ifdef CONFIG_SYS_LOAD_ADDR
	#undef CONFIG_SYS_LOAD_ADDR
	#define CONFIG_SYS_LOAD_ADDR CONFIG_LOADADDR
#endif

#ifdef PHY_ANEG_TIMEOUT
	#undef PHY_ANEG_TIMEOUT
	/* GigaBit link requires longer autonegotiation */
	#define PHY_ANEG_TIMEOUT 10000
#endif

#ifdef INITRD_HIGH
	#undef INITRD_HIGH
#endif
#define INITRD_HIGH "0x50000000"

#ifdef FDT_HIGH
	#undef FDT_HIGH
#endif
#define FDT_HIGH "0x48000000"

#if defined(CONFIG_IMX_HAB)
	/* Platform specific initialization */
	#define PLATFORM_INIT \
		"setenv basebootargs console=${console} rootwait rw rootdelay=1 roottimeout=10 loglevel=4; " \
		"setenv fdt_addr ${loadaddr}; " \
		"part number $plat_dev_if $plat_dev boot plat_boot_part; " \
		"part number $plat_dev_if $plat_dev system plat_system_part; " \
		"usb reset; " \
		"setenv usb_recovery_part 1; " \
		"hab_status"
#else
	/* Platform specific initialization */
	#define PLATFORM_INIT \
		"setenv basebootargs console=${console} rootwait rw rootdelay=1 roottimeout=10 loglevel=4; " \
		"setenv fdt_addr ${loadaddr}; " \
		"part number $plat_dev_if $plat_dev boot plat_boot_part; " \
		"part number $plat_dev_if $plat_dev system plat_system_part; " \
		"usb reset; " \
		"setenv usb_recovery_part 1; "
#endif

/* Platform specific environment settings */
#define BASE_BOARD_CONFIG_EXTRA_ENV_SETTINGS \
	"basebootargs=dummy - see platform_init\0" \
	"plat_dev_if=mmc\0" \
	"plat_dev=0\0" \
	"plat_dev_linux=/dev/mmcblk0p\0" \
	"usb_dev_if=usb\0" \
	"usb_dev=0\0" \
	"get_menu= \0"

/* In case the device does not provide a HID support we offer the menu control via GPIO and a led as feed back. */
#if defined(CONFIG_BOOTMENU_GPIO)
	/* devices with no HI like keyboard may use a gpio for example for boot menu validation */
	#define BOARD_CONFIG_EXTRA_ENV_SETTINGS \
		BASE_BOARD_CONFIG_EXTRA_ENV_SETTINGS \
		"menu_gpio="CONFIG_BOOTMENU_GPIO_CTRL"\0" \
		"led_gpio="CONFIG_BOOTMENU_GPIO_LED"\0" \
		"get_menu="GET_MENU_VAL"\0"

	#define GET_MENU_VAL \
		"gpio input $menu_gpio; " \
		"if test $? -eq 0; then " \
			"gpio input $led_gpio; " \
			"setenv led_stat $?; " \
			"gpio set $led_gpio; " \
			"setenv menu_active 1; " \
			"setenv boot_menu 0; " \
			"while test $menu_active = 1; do " \
				"setenv menu_active 0; " \
				"sleep 2; " \
				"gpio input $menu_gpio; " \
				"if test $? -eq 0; then " \
					"setexpr boot_menu $boot_menu + 1; " \
					"if test $boot_menu -lt $boot_menu_max; then " \
						"setenv menu_active 1; " \
					"fi; " \
					"setenv blink $boot_menu; " \
					"echo menu-counter: $blink; " \
					"while test $blink -gt 0; do " \
						"gpio toogle $led_gpio; " \
						"sleep 1; " \
						"gpio toogle $led_gpio; " \
						"sleep 1; " \
						"setexpr blink $blink - 1; " \
					"done; " \
				"fi; " \
			"done; " \
			"gpio input $led_gpio; " \
			"if test $led_stat != $?; then " \
				"gpio toogle $led_gpio; " \
			"fi; " \
		"fi;"
#else
	/* definition not necesarry since control is done via keyboard, screen, serial... */
	#define BOARD_CONFIG_EXTRA_ENV_SETTINGS \
		BASE_BOARD_CONFIG_EXTRA_ENV_SETTINGS
#endif
