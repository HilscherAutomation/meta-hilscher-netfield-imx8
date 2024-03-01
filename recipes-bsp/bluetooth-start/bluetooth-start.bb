SUMMARY = "Bluetooth startup service"
DESCRIPTION = "Service to startup bluetooth service via UART."
SECTION = "base"

LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
LICENSE = "MIT"
PACKAGE_ARCH = "${MACHINE_ARCH}"

INHIBIT_DEFAULT_DEPS = "1"

SRC_URI = "file://bluetooth-start.service"

inherit systemd

PACKAGES = "${PN}"
SYSTEMD_PACKAGES="${PN}"
SYSTEMD_SERVICE:${PN} = "bluetooth-start.service"
SYSTEMD_AUTO_ENABLE ?= "disable"

do_install() {
    install -d ${D}${systemd_system_unitdir}
    install -m0600 ${WORKDIR}/bluetooth-start.service ${D}${systemd_system_unitdir}
}

FILES:${PN} = "${systemd_system_unitdir}"
