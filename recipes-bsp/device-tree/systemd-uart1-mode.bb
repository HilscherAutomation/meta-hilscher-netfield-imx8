SUMMARY = "Setup uart1 mode (rs485/rs232)"
HOMEPAGE = "www.hilscher.com"
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = ""

inherit systemd

SRC_URI = " \
	file://uart1-mode.service \
	file://uart1-mode.sh \
"

do_install() {
	install -d ${D}/${base_sbindir}
	install -m 755 ${WORKDIR}/uart1-mode.sh "${D}/${base_sbindir}"

	install -d "${D}/${systemd_unitdir}/system/"
	install -m 644 ${WORKDIR}/uart1-mode.service ${D}/${systemd_unitdir}/system/
}

SYSTEMD_SERVICE_${PN} = "uart1-mode.service"

FILES_${PN} = "${base_sbindir}"
FILES_${PN} += "${systemd_unitdir}/system"
