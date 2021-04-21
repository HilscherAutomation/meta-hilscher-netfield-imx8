FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI_append += "file://dont_use_memset.patch"
