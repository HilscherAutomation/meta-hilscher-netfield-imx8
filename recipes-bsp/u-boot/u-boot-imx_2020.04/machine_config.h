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
	#define PLATFORM_INIT "hab_status"
#else
	#define PLATFORM_INIT " "
#endif

/* In case the device does not provide a HID support we offer the menu control via GPIO and a led as feed back. */
#if defined(CONFIG_BOOTMENU_GPIO)
	/* devices with no HI like keyboard may use a gpio for example for boot menu validation */
	#define BOARD_CONFIG_EXTRA_ENV_SETTINGS \
		"mmcdev=1\0" \
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
		"get_menu= \0"
#endif
