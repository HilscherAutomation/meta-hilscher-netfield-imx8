FILESEXTRAPATHS:prepend := "${TOPDIR}/../meta-bsp-imx8mm/recipes-kernel/linux/compulab/5.15/imx8mm:"

require recipes-kernel/linux/compulab/5.15/imx8mm.inc

do_configure:append () {
    mkdir ${S}/firmware/brcm -p
    cp ${WORKDIR}/recipe-sysroot/${base_libdir}/firmware/brcm/bcm4339/4339.hcd ${S}/firmware/brcm/BCM4335C0.hcd

    # Comment out this line to get the configuration fragments working!
    #oe_runmake ${MACHINE}_defconfig
}

KERNEL_MODULE_AUTOLOAD += "goodix"

DEPENDS += "firmware-imx"
