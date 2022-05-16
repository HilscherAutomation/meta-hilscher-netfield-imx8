#SOC_FAMILY = "mx8:mx8m:mx8mm"
require conf/machine/include/soc-family.inc

# This is required by imx-boot 0.2.bb, because this recipe overrides the SOC_FAMILY variable!
MACHINEOVERRIDES =. "mx8:mx8m:mx8mm:"

require conf/machine/include/imx-base.inc
#require conf/machine/include/arm/arch-arm64.inc
require conf/machine/include/tune-cortexa53.inc

IMAGE_CLASSES_append += "hilscher_image_types"
FASTBOOT_SUPPORT = "1"
IMAGE_FSTYPES += "wic.bz2 swu ${@'fastboot' if d.getVar('FASTBOOT_SUPPORT')=='1' else ''} licenses.tar.bz2"
SOC_DEFAULT_IMAGE_FSTYPES = ""

# meta-bsp-imx8mm adds git-lfs and bison to hosttools which is not needed / working in our docker env
HOSTTOOLS_remove += "git-lfs bison"
# meta-bsp-imx8mm adds firmware-imx-sdma if distro != dunfell which is unavailable in netFIELD which is based on dunfell
CORE_IMAGE_EXTRA_INSTALL_remove += "firmware-imx-sdma"
# meta-bsp-imx8mm adds kernel image to all images which makes core-image-minimal-initramfs too large
IMAGE_INSTALL_remove += "kernel-image kernel-modules kernel-devicetree"

#############################################################################
# Preferred components/settings
#############################################################################
PREFERRED_PROVIDER_virtual/dtb ??= "device-tree"
MACHINE_ESSENTIAL_EXTRA_RDEPENDS_append += "device-tree"

IMX_DEFAULT_BSP = "nxp"
PREFERRED_PROVIDER_virtual/kernel = "linux-fslc-imx"

# Note: INITRAMFS_IMAGE must only be defined weakly!
INITRAMFS_IMAGE ?= "core-image-minimal-initramfs"
HILSCHER_RESCUE_IMAGE ??= "hilscher-rescue-image"
IMAGE_FSTYPES_remove_pn-hilscher-rescue-image += "squashfs_signed"
IMAGE_FSTYPES_append_pn-hilscher-rescue-image += "squashfs_xz_signed"

BSP_DEPLOYSCRIPT_DIR ??= "${DYNAMIC_LAYER_DIR_BASE}/meta-freescale/scripts/deploy"

SERIAL_CONSOLES = "115200;ttymxc2"

# Don't build USB images (.zip) and recovery SWU
NETFIELD_IMAGES = ""

MACHINE_FEATURES += "bluetooth optee tpm2"
MACHINE_SOCARCH_FILTER_append_mx8mm = "virtual/libopenvg virtual/libgles1 virtual/libgles2 virtual/egl virtual/mesa virtual/libgl virtual/libg2d"
MACHINE_FIRMWARE += "linux-firmware-bcm4339 linux-firmware-imx-sdma-imx7d"

CIFX_SPI_CONFIGS ?= "Device=spidev2.0;Speed=10000000;Mode=3;ChunkSize=0"

SYSTEMD_DEFAULT_TARGET ??= "system-up.target"
MACHINE_ESSENTIAL_EXTRA_RDEPENDS_append += "system-up"

MACHINE_ESSENTIAL_EXTRA_RDEPENDS_append += "${@bb.utils.contains('MACHINE_FEATURES', 'hab', 'hab-signature', '', d)}"

#############################################################################
# Keys
#############################################################################
PLATFORM_SIGN ??= "1"
PLATFORM_KEYDIR ??= "${KEYS_DIR}"
PLATFORM_KEYNAME ??= "boot"

KEYS_IMAGE_SIGN_PRIV ??= "${KEYS_DIR}/boot.key"
KEYS_IMAGE_SIGN_CERT ??= "${KEYS_DIR}/boot.crt"
KEYS_IMAGE_SIGN_CERT_DER ??= "${KEYS_DIR}/boot.crt.der"

FIT_HASH_ALG = "sha256"
FIT_SIGN_ALG = "rsa4096"

#############################################################################
# Bootloader
#############################################################################
PREFERRED_PROVIDER_virtual/bootloader="u-boot-imx"
WKS_FILE_DEPENDS_remove_mx8 += "imx-boot"

IMX_BOOT_FILE = "imx-boot"
IMX_BOOT_FILE .= "${@bb.utils.contains('MACHINE_FEATURES', 'hab', '.signed', '', d)}"
IMX_BOOT_SEEK = "33"

UBOOT_MACHINE = "${MACHINE}_defconfig"
UBOOT_BINARY = "flash.bin"
UBOOT_MAKE_TARGET_mx8 = "flash.bin"

# NOTE: This is commented out as it is defaulting now.
#UBOOT_DTB_NAME = "${MACHINE}.dtb"

# Set DDR FIRMWARE required by u-boot-spl
DDR_FIRMWARE_NAME = "lpddr4_pmu_train_1d_imem.bin lpddr4_pmu_train_1d_dmem.bin lpddr4_pmu_train_2d_imem.bin lpddr4_pmu_train_2d_dmem.bin"

UBOOT_ENTRYPOINT = "0x40008000"
UBOOT_DTB_BINARY = ""
UBOOT_SIGN_ENABLE = "1"
UBOOT_SIGN_KEYDIR = "${KEYS_DIR}/"
UBOOT_SIGN_KEYNAME = "boot"

#############################################################################
# Kernel
#############################################################################
KERNEL_CLASSES_append += "kernel-fitimage"
KERNEL_IMAGETYPES_append += "fitImage"

#############################################################################
# Contents of boot partition (excluding rootfs)
#############################################################################
IMAGE_BOOT_FILES  = "${IMX_BOOT_FILE};boot.bin"

# depending on the image settings choose the correct boot script
SCRIPT_SUFFIX = "${@bb.utils.contains('IMAGE_FEATURES', 'debug-tweaks', '-debug.scr', "-fastboot.scr" ,d)}"
IMAGE_BOOT_FILES += "boot-script-fit/boot-menu${SCRIPT_SUFFIX};boot.scr"

#############################################################################
# WIC images
#############################################################################
WKS_FILE = "hilscher-gpt-boot-rescue-system-lvm.wks.in"

WIC_BOOT_PART_CONTENT ??= "${IMAGE_BOOT_FILES}"
WIC_RESCUE_PART_CONTENT ??= "${HILSCHER_RESCUE_IMAGE_LINK_NAME}.squashfs-xz;${HILSCHER_RESCUE_IMAGE}-${MACHINE}-${IMAGE_VERSION_SUFFIX}.squashfs \
                             rboot.cfg fitImage initrd-api-part-cfg;initrd-api"
WIC_SYSTEM_PART_CONTENT ??= "${IMAGE_LINK_NAME}.squashfs boot.cfg fitImage"

#############################################################################
# SWU images
#############################################################################
SWU_FILES += "${IMX_BOOT_FILE};boot.bin"
SWU_FILE_ATTRIBUTES += "boot.bin;path=\"/dev/mmcblk0\";offset=\"${IMX_BOOT_SEEK}K\";name=\"boot.bin\";version;install-if-different=true"
SWU_SCRIPTS += "swupdate/helper.lua;helper.lua"

SWU_RSYNC_PART_UPDATE ??= "boot system"
SWU_BOOT_PART_CONTENT ??= "${IMAGE_BOOT_FILES}"
SWU_SYSTEM_PART_CONTENT ??= "${IMAGE_LINK_NAME}.squashfs nboot.cfg fitImage"

#############################################################################
# System Partitioning
#############################################################################
# Physical device of system partitions
PHYSICAL_SYSTEM_DEVICE ??= "/dev/mmcblk0 /dev/mmcblk1"
# Size of boot partition
IMAGE_PART_BOOT_SIZE ??= "64M"
# Size of rescue partition
IMAGE_PART_RESCUE_SIZE ??= "128M"
# Size of system partition
IMAGE_PART_SYSTEM_SIZE ??= "1G"
# Size of data partition
IMAGE_PART_DATA_SIZE ??= "max"
# Size of logical volume "data" in userdata partition
PART_DATA_LV_DATA_SIZE ??= "60%VG"
# Size of logical volume "backup" in userdata partition
PART_DATA_LV_BACKUP_SIZE ??= "35%VG"
# Size of logical volume "snapshot" in userdata partition
PART_DATA_LV_SNAPSHOT_SIZE ??= "5%VG"

#############################################################################
# rootfs overlay
#############################################################################
HILSCHER_OVERLAY ??= "/dev/mapper/data-data"
HILSCHER_OVERLAYTARGETS ??= "${NETIOT_OVERLAY_DIRS}"

#############################################################################
# The following BSP repository is required ...
# https://github.com/compulab-yokneam/meta-bsp-imx8mm.git (branch: rel_imx_5.4.24-stable)
#############################################################################
BBMASK += "meta-bsp-imx8mm/recipes-bsp/recipes-images/compulab-ucm-imx8m-mini.bb"
BBMASK += "meta-bsp-imx8mm/recipes-bsp/bt-start/bt-start_%.bbappend"
BBMASK += "meta-bsp-imx8mm/recipes-bsp/u-boot/u-boot-imx-fw-utils_2019.04.bbappend"
BBMASK += "meta-bsp-imx8mm/recipes-bsp/u-boot-scr/u-boot-script.bbappend"

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
