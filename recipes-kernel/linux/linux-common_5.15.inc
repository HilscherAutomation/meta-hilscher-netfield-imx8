FILESEXTRAPATHS:prepend := "${THISDIR}/linux-5.15:${THISDIR}/files:${THISDIR}/compulab:"

require recipes-kernel/linux/netfield-linux.inc

# Prevent automatically inclusion of kernel-image into rootfs/image
RDEPENDS_${KERNEL_PACKAGE_NAME}-base = ""

# Disable SCMVERSION which might produce a mismatch between kernel version and /lib/modules folder
SCMVERSION = "n"

SRC_URI:append = " \
    file://netfieldos.cfg \
"

# Add support for dtso files
SRC_URI:append:netfield-compact-x8m-rev1 = " \
    file://0015-of-Add-dt-overlay-configfs-interface-v2.patch \
    file://of_configfs.cfg \
"
SRC_URI:append:netfield-compact-x8m-rev1 = " file://netfield-compact-x8m-rev1.cfg"

# Add support for yocto-kernel-cache
# ----------------------------------
require recipes-kernel/linux/linux-yocto.inc
KCONFIG_MODE = "--alldefconfig"

KMETA = "kernel-meta"
SRC_URI:append = " git://git.yoctoproject.org/yocto-kernel-cache;type=kmeta;name=meta;branch=yocto-5.15;destsuffix=${KMETA}"
# ----------------------------------

KERNEL_FEATURES:append = " \
  features/bluetooth/bluetooth.scc features/bluetooth/bluetooth-usb.scc \
  features/rfkill/rfkill.scc \
  bsp/nxp-imx8/nxp-imx8.scc \
"
# TPM does not work as TCG_ATMEL is included which results in a kernel crash
KERNEL_FEATURES:remove = " features/tpm/tpm.scc"

SRC_URI:append = " \
	file://0001-imx8mm.dtsi-Add-a-label-to-soc-node.patch \
	file://rfkill_gpio_ofsupport.patch \
	file://disable_msi_if_cifx_found.patch \
	file://tpm.cfg \
	file://led_timer.cfg \
	file://usb.cfg \
	file://enable_abx80x.cfg \
"

# Enable module for qemu support
SRC_URI:append = " file://binfmt_misc.cfg"

# TPM2.0 via I2C support
SRC_URI:append = " file://add_st_tpm2_i2c_support.patch \
                   file://tpm_tcg_i2c.cfg"

SRC_URI:append = " file://prevents_error_messages_related_to_u-boot,gpio-hog_DT_properties.patch"

SRC_URI:append = " file://compulab-rtc-abx80-fix-false-fail-caused-by-osc-glitch.patch"

# ------------------------------------------------------------------------------
export_devicetree_settings() {
	[ "$1" = "-v" ] && verbose=1
	dtb_name="${KERNEL_DEVICETREE}"
	[ -z "$dtb_name" ] && dtb_name="${MACHINE}.dtb"
	dtb_provider="${PREFERRED_PROVIDER_virtual/dtb}"

	if [ -n "$dtb_provider" ]; then
		dtb_count="$(ls ${WORKDIR}/recipe-sysroot/boot/devicetree/*.dtb | wc -l)"
		if [ "$dtb_count" -gt "1" -a -z "$dtb_name" ]; then
			bberror "There are multiple devicetrees provided by virtual/dtb '$dtb_provider'!"
			bbfatal "Please select a specific one by adding 'KERNEL_DEVICETREE' to machine configuration."

		# NOTE: Commented out as only one dtb is currently supported, but the KERNEL_DEVICETREE is required for selection.
		#elif [ "$dtb_count" -eq "1" -a -n "$dtb_name" ]; then
		#	bbwarn "There is only one devicetree provided by virtual/dtb '$dtb_provider'!"
		#	bbwarn "Therfore 'KERNEL_DEVICETREE' is not required and should be removed from configuration."

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

		[ $verbose ] && bbwarn "Use '$dtb_name' as devicetree." || true

	else
		#bbfatal "Invalid or missing devicetree, please check 'PREFERRED_PROVIDER_virtual/dtb' and 'KERNEL_DEVICETREE'!"
		[ $verbose ] && bbwarn "Use dtb-file defined by machine defconfig." || true
	fi
}

do_compile[depends] += "virtual/dtb:do_populate_sysroot"
do_compile:prepend() {
	export_devicetree_settings -v

	# Create links to external provided DTB files.
	if [ -n "${EXT_DTB}" ]; then
		mkdir -p ${B}/arch/arm64/boot/dts
		ln -sf ${EXT_DTB} ${B}/arch/arm64/boot/dts/$(basename ${EXT_DTB})
	fi
}
