COMPATIBLE_HOST = "aarch64-.*-linux"

# needed during production process (DMA module is required by the spidev driver used by the netX driver)
PACKAGE_INSTALL:append = " kernel-module-imx-sdma linux-firmware-imx-sdma-imx7d"

# needed for wifi card ax200
PACKAGE_INSTALL:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'wifi', 'kernel-module-iwlwifi kernel-module-iwlmvm wireless-regdb linux-firmware-ax200', '', d)}"
