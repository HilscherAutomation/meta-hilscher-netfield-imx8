COMPATIBLE_HOST = "aarch64-.*-linux"

# needed during production process (DMA module is required by the spidev driver used by the netX driver)
PACKAGE_INSTALL_append += "kernel-module-imx-sdma linux-firmware-imx-sdma-imx7d"
