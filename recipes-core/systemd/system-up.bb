SUMMARY = "LEDs are used to inform about systemd has started all services!"
HOMEPAGE = "https://www.hilscher.com"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/GPL-2.0-only;md5=801f80980d171dd6425610833a22dbe6"

SRC_URI = " \
	file://system-up.target \
	file://system-status-led.service \
	file://system-status-led \
"

S = "${WORKDIR}"

# As this package is tied to systemd, only build it when we're also building systemd.
inherit features_check
REQUIRED_DISTRO_FEATURES = "systemd"

inherit systemd

do_install() {
	install -d ${D}${systemd_unitdir}/system/
	install -m 0644 ${WORKDIR}/system-up.target ${D}${systemd_unitdir}/system/
	install -m 0644 ${WORKDIR}/system-status-led.service ${D}${systemd_unitdir}/system/

	install -d ${D}/${bindir}
	install -m 755 ${WORKDIR}/system-status-led ${D}/${bindir}/
}

FILES:${PN} = "${systemd_unitdir} ${sysconfdir} ${bindir}"

SYSTEMD_SERVICE:${PN} = "system-status-led.service"

# This package includes a machine specific script file!
PACKAGE_ARCH = "${MACHINE_ARCH}"
