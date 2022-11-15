FILESEXTRAPATHS_prepend := "${THISDIR}/${BPN}:"

# Required by *platform_init*
RDEPENDS_${PN}-platform-init_append_netfield-compact-x8m-rev1 += "i2c-tools device-tree"

SRC_URI_append += "file://platform_restore"

do_install_append() {
    install -d ${D}${sbindir}
    install ${WORKDIR}/platform_restore ${D}${sbindir}
}

FILES_${PN}_append += "${sbindir}/platform_restore"
