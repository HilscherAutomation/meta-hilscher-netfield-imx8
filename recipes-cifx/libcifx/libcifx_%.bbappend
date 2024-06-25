PACKAGECONFIG:append = " tun spm"

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI:append:netfield-iolink-edge-gw-revx = " file://skip_firmware_ident.patch"
