# Make sure os-release is machine-specific for netfield-iolink-edge-gw-revx
# see allarch.bbclass
python allarch_package_arch_handler () {
    return
}
PACKAGE_ARCH = "all"
PACKAGE_ARCH:netfield-iolink-edge-gw-revx = "${MACHINE_ARCH}"
VARIANT_ID:netfield-iolink-edge-gw-revx = "sensoredge"
OS_RELEASE_FIELDS:append:netfield-iolink-edge-gw-revx = " VARIANT_ID"

