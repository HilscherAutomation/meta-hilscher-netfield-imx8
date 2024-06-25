FILESEXTRAPATHS:prepend := "${TOPDIR}/../meta-bsp-imx8mm/recipes-kernel/linux/compulab/imx8mm:"

require recipes-kernel/linux/compulab/imx8mm.inc

DEPENDS += "firmware-imx"
