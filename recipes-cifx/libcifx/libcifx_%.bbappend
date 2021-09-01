PACKAGECONFIG_append += "tun spm"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
SRC_URI_append_netfield-iolink-edge-gw-revx += "file://skip_firmware_ident.patch"
