COMPATIBLE_HOST = "aarch64-.*-linux"

# sdma driver might already be loaded / initialized in initrd, so provide the firmware as well
PACKAGE_INSTALL_append += "linux-firmware-imx-sdma-imx7d"
