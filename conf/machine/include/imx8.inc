SOC_FAMILY = "mx8mm:mx8-nxp-bsp"
require conf/machine/include/soc-family.inc

MACHINEOVERRIDES =. "imx-boot-container:mx8:mx8m:mx8mm:compulab-mx8mm:"

require conf/machine/include/imx-base.inc
require conf/machine/include/arm/armv8a/tune-cortexa53.inc

# Set imx-mkimage boot target
IMXBOOT_TARGETS = "flash_evk"
IMXBOOT_TARGETS_BASENAME = "flash_evk"
IMAGE_BOOTLOADER = "imx-boot"

# Optee parameters
OPTEE_BIN_EXT = "8mm"
# Variables need to be set to:${MACHINE} as setting mx8-nxp-bsp
# in SOC_FAMILY results in :mx8mm variables being used instead of
# "normal" variables
OPTEE_PLATFORM_FLAVOR:${MACHINE} = "mx8mm_cl_iot_gate"
PLATFORM_FLAVOR:${MACHINE} = "${OPTEE_PLATFORM_FLAVOR}"
TEE_LOAD_ADDR = "0x56000000"

PLATFORM = "imx8mm"
ATF_MACHINE_NAME = "bl31-imx8mm.bin"
ATF_PLATFORM = "imx8mm"
ATF_LOAD_ADDR = "0x920000"

SOC_TARGET = "iMX8MM"
IMX_BOOT_SOC_TARGET = "iMX8MM"
DEFAULTTUNE = "aarch64"

IMAGE_CLASSES:append = " hilscher_image_types"
IMAGE_FSTYPES += "wic.bz2 swu ${@'fastboot' if d.getVar('FASTBOOT_SUPPORT')=='1' else ''} licenses.tar.bz2"
SOC_DEFAULT_IMAGE_FSTYPES = ""

# meta-bsp-imx8mm adds git-lfs and bison to hosttools which is not needed / working in our docker env
HOSTTOOLS:remove = " git-lfs bison"
# Remove extra installations from meta-imx8mm-bsp's layer.conf
CORE_IMAGE_EXTRA_INSTALL:remove = " \
    kernel-modules kernel linux-compulab-headers kernel-dev \
    packagegroup-tools-bluetooth grub firmware-imx-sdma-imx7d \
"
CORE_IMAGE_EXTRA_INSTALL:append = " linux-firmware-imx-sdma-imx7d"

# meta-bsp-imx8mm adds kernel image to all images which makes core-image-minimal-initramfs too large
IMAGE_INSTALL:remove = " kernel-image kernel-modules kernel-devicetree"

#############################################################################
# Preferred components/settings
#############################################################################
PREFERRED_PROVIDER_virtual/dtb ??= "device-tree"
MACHINE_ESSENTIAL_EXTRA_RDEPENDS:append = " device-tree"

IMX_DEFAULT_BSP = "nxp"
PREFERRED_PROVIDER_virtual/kernel = "linux-fslc-imx"
PREFERRED_VERSION_imx-atf = "2.6%"

# Note: INITRAMFS_IMAGE must only be defined weakly!
INITRAMFS_IMAGE ?= "core-image-minimal-initramfs"
HILSCHER_RESCUE_IMAGE ??= "hilscher-rescue-image"
IMAGE_FSTYPES:remove:pn-hilscher-rescue-image = " squashfs_signed"
IMAGE_FSTYPES:append:pn-hilscher-rescue-image = " squashfs_xz_signed"

BSP_DEPLOYSCRIPT_DIR ??= "${DYNAMIC_LAYER_DIR_BASE}/meta-freescale/scripts/deploy"

SERIAL_CONSOLES = "115200;ttymxc2"

MACHINE_FEATURES += "bluetooth optee tpm2"
MACHINE_FIRMWARE:append = " linux-firmware-bcm4339 linux-firmware-ax200"

CIFX_SPI_CONFIGS ?= "Device=spidev2.0;Speed=10000000;Mode=3;ChunkSize=0"

SYSTEMD_DEFAULT_TARGET ??= "system-up.target"
MACHINE_ESSENTIAL_EXTRA_RDEPENDS:append = " system-up"

MACHINE_ESSENTIAL_EXTRA_RDEPENDS:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'hab', 'hab-signature', '', d)}"
ADDITIONAL_USB_FILES = "${@bb.utils.contains('MACHINE_FEATURES', 'hab', 'boot-2.3-recovery.scr;boot.scr', '', d)}"

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
IMX_DEFAULT_BOOTLOADER="u-boot-compulab"
PREFERRED_PROVIDER_virtual/bootloader="${IMX_DEFAULT_BOOTLOADER}"

IMX_EXTRA_FIRMWARE = "firmware-imx-8m"

IMX_BOOT_FILE = "imx-boot"
IMX_BOOT_FILE .= "${@bb.utils.contains('MACHINE_FEATURES', 'hab', '.signed', '', d)}"
IMX_BOOT_SEEK = "33"

UBOOT_CONFIG ??= "sd"
UBOOT_CONFIG[sd] = "${MACHINE}_defconfig,sdcard"
UBOOT_CONFIG[mfgtool] = "${MACHINE}_defconfig,sdcard"

UBOOT_SUFFIX = "bin"
UBOOT_BINARY = "flash.${UBOOT_SUFFIX}"
UBOOT_MAKE_TARGET = "${UBOOT_BINARY}"

UBOOT_DTB_NAME = "${MACHINE}.dtb"

# Set DDR FIRMWARE required by u-boot-spl
DDR_FIRMWARE_NAME = "lpddr4_pmu_train_1d_imem.bin lpddr4_pmu_train_1d_dmem.bin lpddr4_pmu_train_2d_imem.bin lpddr4_pmu_train_2d_dmem.bin"

# Fix kernel warning by aligning kernel to 2MB boundary (default = 0x40480000)
#  See [Firmware Bug]: Kernel image misaligned at boot, please fix your bootloader!
UBOOT_ENTRYPOINT:${MACHINE} = "0x40400000"
UBOOT_DTB_BINARY = ""
UBOOT_SIGN_ENABLE = "1"
UBOOT_SIGN_KEYDIR = "${KEYS_DIR}/"
UBOOT_SIGN_KEYNAME = "boot"

SPL_BINARY = "spl/u-boot-spl.bin"

#############################################################################
# Kernel
#############################################################################
KERNEL_CLASSES:append = " kernel-fitimage"
KERNEL_IMAGETYPES:append = " fitImage"

#############################################################################
# Contents of boot partition (excluding rootfs)
#############################################################################
IMAGE_BOOT_FILES  = "${IMX_BOOT_FILE};boot.bin"

# depending on the image settings choose the correct boot script
BOOTSCRIPT = "fitImage-boot-menu${@'-fastboot' if d.getVar('FASTBOOT_SUPPORT')=='1' else ''}"
SCRIPT_SUFFIX = "${@bb.utils.contains('IMAGE_FEATURES', 'debug-tweaks', '-console.scr', ".scr" ,d)}"
IMAGE_BOOT_FILES += "boot-script-fit/${BOOTSCRIPT}${SCRIPT_SUFFIX};boot-fit.scr"

#############################################################################
# WIC images
#############################################################################
WKS_FILE = "hilscher-gpt-boot-rescue-system-lvm.wks.in"

WIC_BOOT_PART_CONTENT ??= ""
WIC_RESCUE_PART_CONTENT ??= ""
WIC_SYSTEM_PART_CONTENT ??= ""

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
MACHINE_ESSENTIAL_EXTRA_RDEPENDS:append = " libcifx cifxtun"
