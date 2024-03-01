FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append = " file://dont_use_memset.patch"
