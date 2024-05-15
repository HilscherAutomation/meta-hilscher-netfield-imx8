FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append = " file://dont_use_memset.patch \
                   file://fix_compile_on_linux_5.15.patch"
