FILESEXTRAPATHS_prepend := "${THISDIR}/${BPN}:"

# Required by *platform_init*
RDEPENDS_${PN}-platform-init_append_netfield-compact-x8m-rev1 += "i2c-tools device-tree"
