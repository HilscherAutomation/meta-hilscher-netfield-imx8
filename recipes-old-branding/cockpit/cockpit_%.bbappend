FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

PACKAGE_ARCH="${MACHINE_ARCH}"

SRC_URI_append_netfield-iolink-edge-gw-revx += "file://terminal_override.json"

do_install_append_netfield-iolink-edge-gw-revx() {
    if [ -d "${D}${datadir}/cockpit/terminal" ]; then
        # terminal was split out of systemd package, so we can simply remove it
         rm -rf ${D}${datadir}/cockpit/terminal
    else
        install -m0644 ${WORKDIR}/terminal_override.json ${D}${datadir}/cockpit/systemd/override.json
        rm ${D}${datadir}/cockpit/systemd/terminal.*
    fi
}
