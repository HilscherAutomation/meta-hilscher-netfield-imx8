FILESEXTRAPATHS_prepend := "${THISDIR}/${BPN}:"

SRC_URI_append += "file://lvm_exclude"

do_install_append() {
    install -d ${D}${bindir}
    sed -e 's;@PHYSICAL_SYSTEM_DEVICE@;${PHYSICAL_SYSTEM_DEVICE};' ${WORKDIR}/lvm_exclude > ${D}${bindir}/lvm_exclude
    chmod +x ${D}${bindir}/lvm_exclude
}

FILES_${PN}_append += "${bindir}"
