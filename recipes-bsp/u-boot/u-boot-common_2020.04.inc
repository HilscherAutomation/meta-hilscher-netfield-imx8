FILESEXTRAPATHS_prepend := "${THISDIR}/u-boot-imx_2020.04:"

DEPENDS_append += "u-boot-tools-native"

require recipes-bsp/u-boot/u-boot-netfield.inc

SRC_URI_append += " \
	file://rtl8211e_backport.patch \
	file://1001-devicetree-Skip-devicetree-building-when-EXT_DTB-is-.patch \
	file://1002-clk-composite-Fix-memory-collision-issues.patch \
	file://1003-compulab-imx8mm-Add-Hilscher-enviroments.patch \
	file://1004-compulab-imx8mm-Dependent-to-EEprom-layout-up-to-4-M.patch \
	file://1005-compulab-imx8mm-Add-LED-support-as-defined-in-the-fo.patch \
	file://1006-fix_fastboot_parameter_setup.patch \
	file://1008-fix_phy_initialization.patch \
	file://1009-atheros.c-Fix-the-vddio-1p8v-and-smartEEE-quirk-of-A.patch \
	file://1011-drivers-net-phy-Add-devicetree-support-for-the-max-s.patch \
	file://1012-cmd-mii-Fix-wrong-mii-info-message-related-to-PHY-st.patch \
	file://compulab-iotgate-imx8-add-hilscher-env.patch \
	file://compulab-iotgate-imx8-disable-module-detection.patch \
	file://pxe_add_bootargs_append_func.patch \
	file://fat_show_files_without_arch_attr.patch \
	file://machine_config.h \
	file://defconfig \
	file://0001-Changed-config-file-from-machine-to-distro-specific-.patch \
	file://add_gpio_controlled_boot_menu.patch \
	${@bb.utils.contains('MACHINE_FEATURES', 'u-boot-gpio-menu', 'file://gpio-menu.cfg', '' ,d)} \
	"

SRC_URI_append_netfield-iolink-edge-gw-revx += "file://netfield-iolink-edge-gw-revx.cfg"

export_devicetree_settings() {
	[ "$1" = "-v" ] && verbose=1
	dtb_name="${UBOOT_DTB_NAME}"
	[ -z "$dtb_name" ] && dtb_name="${KERNEL_DEVICETREE}"
	[ -z "$dtb_name" ] && dtb_name="${MACHINE}.dtb"
	dtb_provider="${PREFERRED_PROVIDER_virtual/dtb}"

	if [ -n "$dtb_provider" ]; then
		dtb_count="$(ls ${WORKDIR}/recipe-sysroot/boot/devicetree/*.dtb | wc -l)"
		if [ "$dtb_count" -gt "1" -a -z "$dtb_name" ]; then
			bberror "There are multiple devicetrees provided by virtual/dtb '$dtb_provider'!"
			bbfatal "Please select a specific one by adding 'UBOOT_DTB_NAME' to machine configuration."

		# NOTE: Commented out as only one dtb is currently supported, but the UBOOT_DTB_NAME is required for selection.
		#elif [ "$dtb_count" -eq "1" -a -n "$dtb_name" ]; then
		#	bbwarn "There is only one devicetree provided by virtual/dtb '$dtb_provider'!"
		#	bbwarn "Therfore 'UBOOT_DTB_NAME' is not required and should be removed from configuration."

		fi
		if [ -n "$dtb_name" ]; then
			# Choosing the given dtb file
			dtb_file="$(ls ${WORKDIR}/recipe-sysroot/boot/devicetree/$dtb_name 2>/dev/null || true)"
			[ -z "$dtb_file" ] && bbfatal "$dtb_name not provided by virtual/dtb '$dtb_provider'!"
		else
			# Choosing the first dtb file provided by PREFERRED_PROVIDER_virtual/dtb
			dtb_file="$(ls ${WORKDIR}/recipe-sysroot/boot/devicetree/*.dtb 2>/dev/null | head -n1)"
			[ -z "dtb_file" ] && bbfatal "No dtb-file provided by virtual/dtb '$dtb_provider'!"
		fi

		# Export as external devicetree
		export EXT_DTB="$dtb_file"
		[ $verbose ] && bbwarn "Use external provided '$(basename $dtb_name)' as devicetree." || true

	elif [ -n "$dtb_name" ]; then

		# Remove file extension and export as internal devicetree
		export DEVICE_TREE="${dtb_name%.*}"
		[ $verbose ] && bbwarn "Use '$dtb_name' as devicetree." || true

	else
		#bbfatal "Invalid or missing devicetree, please check 'PREFERRED_PROVIDER_virtual/dtb' and 'UBOOT_DTB_NAME'!"
		[ $verbose ] && bbwarn "Use dtb-file defined by machine defconfig." || true
	fi
}

do_configure[depends] += "virtual/dtb:do_populate_sysroot"
do_configure_prepend() {
	cp ${WORKDIR}/machine_config.h       ${S}/include/configs/

	export_devicetree_settings -v

	# Replace/Create machine defconfig by defconfig provided by this recipe.
	for config in ${UBOOT_MACHINE}; do
		cp -f ${WORKDIR}/${config} ${S}/configs || true
		ls ${S}/configs/${config} || cp -f ${WORKDIR}/defconfig ${S}/configs/$config
		ls ${S}/configs/${config} || bbfatal "Invalid or missing $config"
	done

	# Patch all machine defconfig file, because the $(U_BOOT_ITS) target use CONFIG_OF_LIST
	# which depends on CONFIG_DEFAULT_DEVICE_TREE.
	if [ -n "${EXT_DTB}" -o "$DEVICE_TREE" ]; then
		for config in ${UBOOT_MACHINE}; do
			sed -i -e 's,.*\(CONFIG_DEFAULT_DTB\).*,\1="'$(basename ${EXT_DTB})'",g' ${S}/configs/$config
			sed -i -e 's,.*\(CONFIG_DEFAULT_FDT_FILE\).*,\1="'$(basename ${EXT_DTB})'",g' ${S}/configs/$config
			sed -i -e 's,.*\(CONFIG_DEFAULT_DEVICE_TREE\).*,\1="'$(basename ${EXT_DTB} .dtb)'",g' ${S}/configs/$config
		done
	fi

	# Create links to external provided DTB files.
	if [ -n "${EXT_DTB}" ]; then
		if [ -n "${UBOOT_CONFIG}" ]; then
			for config in ${UBOOT_MACHINE}; do
				mkdir -p ${B}/${config}/arch/arm/dts
				ln -sf ${EXT_DTB} ${B}/${config}/arch/arm/dts/$(basename ${EXT_DTB})
			done
		else
			mkdir -p ${B}/arch/arm/dts
			ln -sf ${EXT_DTB} ${B}/arch/arm/dts/$(basename ${EXT_DTB})
		fi
	fi
}

PLATFORM        ?= "INVALID"
PLATFORM_mx8qm   = "imx8qm"
PLATFORM_mx8qxp  = "imx8qxp"
PLATFORM_mx8mq   = "imx8mq"
PLATFORM_mx8mm   = "imx8mm"

BOOT_TOOLS ??= "imx-boot-tools"

do_compile[depends] += "imx-atf:do_deploy firmware-imx-8m:do_deploy \
                        ${@bb.utils.contains('MACHINE_FEATURES', 'optee', 'optee-os:do_deploy', '', d)}"
do_compile_prepend() {
	if [ -n "${UBOOT_CONFIG}" ]; then
		for config in ${UBOOT_MACHINE}; do
			if [ "${@bb.utils.contains('MACHINE_FEATURES', 'optee', 'true', 'false',d)}" = "true" ]; then
				# Copy ARM Trusted Firmware to build directory
				cp ${DEPLOY_DIR_IMAGE}/${BOOT_TOOLS}/bl31-${PLATFORM}.bin-optee ${B}/${config}/bl31.bin
				cp ${DEPLOY_DIR_IMAGE}/ tee.bin  ${B}/${config}/tee.bin
			else
				# Copy ARM Trusted Firmware to build directory
				cp ${DEPLOY_DIR_IMAGE}/${BOOT_TOOLS}/bl31-${PLATFORM}.bin ${B}/${config}/bl31.bin
			fi

			# Copy DDR training firmware to source directory
			for ddr_firmware in ${DDR_FIRMWARE_NAME}; do
				cp -f ${DEPLOY_DIR_IMAGE}/${ddr_firmware} ${B}/${config}
			done
		done
	else
		if [ "${@bb.utils.contains('MACHINE_FEATURES', 'optee', 'true', 'false',d)}" = "true" ]; then
			# Copy ARM Trusted Firmware to build directory
			cp ${DEPLOY_DIR_IMAGE}/${BOOT_TOOLS}/bl31-${PLATFORM}.bin-optee ${B}/bl31.bin
			cp ${DEPLOY_DIR_IMAGE}/tee.bin ${B}/tee.bin
		else
			# Copy ARM Trusted Firmware to build directory
			cp ${DEPLOY_DIR_IMAGE}/${BOOT_TOOLS}/bl31-${PLATFORM}.bin ${B}/bl31.bin
		fi

		# Copy DDR training firmware to source directory
		for ddr_firmware in ${DDR_FIRMWARE_NAME}; do
			cp -f ${DEPLOY_DIR_IMAGE}/${ddr_firmware} ${B}
		done
	fi

	export_devicetree_settings
}

do_compile_prepend_mx8mm() {
	# for imx8mm change the ATF and TEE load address
	export ATF_LOAD_ADDR=0x00920000
	export TEE_LOAD_ADDR=0x7e000000
}

do_deploy_append() {
    # in case "hab" (secure boot) is enabled, meta-hab will create the correct link to the signed bootloader image
    if [ "${@bb.utils.contains('MACHINE_FEATURES', 'hab', 'true', 'false',d)}" = "false" ]; then
        ln -sf flash.bin ${DEPLOYDIR}/${IMX_BOOT_FILE}
    fi
}

inherit hilscher-deploy

hd_path = "${HDEPLOY_PATH_EXTRAS}/bootloader"

do_hilscher_deploy() {
	cd ${DEPLOYDIR}
	cp -a $(readlink flash.bin) ${hd_path}/
}
do_hilscher_deploy[cleandirs] = "${hd_path}/"
addtask hilscher_deploy before do_build after do_deploy
