FILESEXTRAPATHS_prepend := "${TOPDIR}/../meta-bsp-imx8mm/recipes-kernel/linux/compulab/imx8mm:"

require recipes-kernel/linux/compulab/imx8mm.inc

do_configure_append () {
    mkdir ${S}/firmware/brcm -p
    cp ${WORKDIR}/recipe-sysroot/${base_libdir}/firmware/brcm/bcm4339/4339.hcd ${S}/firmware/brcm/BCM4335C0.hcd
    
    # Comment out this line to get the configuration fragments working!
    #oe_runmake ${MACHINE}_defconfig
}

KERNEL_MODULE_AUTOLOAD += "goodix"

COMPATIBLE_MACHINE = "(ucm-imx8m-mini|mcm-imx8m-mini|iot-gate-imx8)"

DEPENDS += "firmware-imx"
