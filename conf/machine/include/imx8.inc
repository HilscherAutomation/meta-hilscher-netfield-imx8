#SOC_FAMILY = "mx8:mx8m:mx8mm"
require conf/machine/include/soc-family.inc

# This is required by imx-boot 0.2.bb, because this recipe overrides the SOC_FAMILY variable!
MACHINEOVERRIDES =. "mx8:mx8m:mx8mm:"

require conf/machine/include/imx-base.inc
#require conf/machine/include/arm/arch-arm64.inc
require conf/machine/include/tune-cortexa53.inc

require conf/machine/hilscher-machine-vars.conf

IMX_DEFAULT_BSP = "nxp"
PREFERRED_PROVIDER_virtual/kernel = "linux-fslc-imx"

#############################################################################
# Bootloader
#############################################################################
PREFERRED_PROVIDER_virtual/bootloader="u-boot-imx"
WKS_FILE_DEPENDS_remove_mx8 += "imx-boot"

IMX_BOOT_FILE = "imx-boot"
IMX_BOOT_SEEK = "33"

#############################################################################
# Kernel
#############################################################################
KERNEL_CLASSES += "kernel-fitimage"
KERNEL_IMAGETYPES_append += "fitImage"

#############################################################################
# Build fastboot images
#############################################################################
SOC_DEFAULT_IMAGE_FSTYPES = ""
IMAGE_FSTYPES += "fastboot"

#############################################################################
# Contents of boot partition (excluding rootfs)
#############################################################################
IMAGE_BOOT_FILES = "boot.scr ${IMX_BOOT_FILE};boot.bin"

#############################################################################
# WIC images
#############################################################################
WKS_SEARCH_PATH_prepend = "${DYNAMIC_LAYER_DIR_BASE}/meta-freescale/files/part-schemes/${PART_SCHEME}:"

WIC_EXTRA_LINES = "bootloader --ptable gpt"

WIC_BOOT_PART_BASE_CONTENT ?= "${IMAGE_BOOT_FILES}"
WIC_BOOT_PART_EXTRA_CONTENT ?= ""

#############################################################################
# SWU images
#############################################################################
SWU_FILES ?= "${IMX_BOOT_FILE};boot.bin"
SWU_FILE_ATTRIBUTES ?= "boot.bin;path=\"/dev/mmcblk0\";offset=\"${IMX_BOOT_SEEK}K\";name=\"boot.bin\";version;install-if-different=true"

#############################################################################
# The following BSP repository is required ...
# https://github.com/compulab-yokneam/meta-bsp-imx8mm.git (branch: rel_imx_5.4.24-stable)
#############################################################################
BBMASK += "meta-bsp-imx8mm/recipes-bsp/recipes-images/compulab-ucm-imx8m-mini.bb"
BBMASK += "meta-bsp-imx8mm/recipes-bsp/bt-start/bt-start_%.bbappend"
BBMASK += "meta-bsp-imx8mm/recipes-bsp/u-boot/u-boot-imx-fw-utils_2019.04.bbappend"

BBMASK += "meta-bsp-imx8mm/recipes-bsp/recipes-images/base-files_%.bbappend"
BBMASK += "meta-bsp-imx8mm/recipes-kernel/linux/linux-imx_5.4.%.bbappend"

#############################################################################
# Essential firmware files for UCM i.MX8MM module
#############################################################################
# Firmware for ucm-imx8m-mini onboard WiFi module
MACHINE_FIRMWARE += "linux-firmware-bcm4329-fullmac linux-firmware-bcm4329"
# Required for some qualcomm BT sticks
MACHINE_FIRMWARE += "linux-firmware-qca"
# netX90 communication
MACHINE_ESSENTIAL_EXTRA_RDEPENDS_append += "libcifx cifxtun"
