SUMMARY = "Hilscher BSP device trees"
DESCRIPTION = "Hilscher BSP device trees from within layer."
SECTION = "bsp"

# the device trees from within the layer are licensed as MIT, kernel includes are GPL
LICENSE = "MIT & GPL-2.0-only"
LIC_FILES_CHKSUM = " \
	file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302 \
	file://${COMMON_LICENSE_DIR}/GPL-2.0-only;md5=801f80980d171dd6425610833a22dbe6 \
"

inherit devicetree

inherit dts-sign
# Setup public key patching into dts
DTS_SIGN_ENFORCE="${PLATFORM_SIGN}"
DTS_SIGN_KEY_DIR="${PLATFORM_KEYDIR}"
DTS_SIGN_KEY_NAME="${PLATFORM_KEYNAME}"

python do_apply_verification_keys:prepend() {
  if d.getVar('DTS_TO_SIGN') is None:
    dts_to_sign = d.getVar('KERNEL_DEVICETREE') or d.getVar('MACHINE')
    dts_to_sign = dts_to_sign.replace('.dtb','')+".dts"
    d.setVar('DTS_TO_SIGN', "${WORKDIR}/src/"+dts_to_sign)
}

do_unpack[vardeps] += "PLATFORM_SIGN PLATFORM_KEYDIR PLATFORM_KEYNAME"

S = "${WORKDIR}/src"

devicetree_do_install:append() {
	default_dtb="${KERNEL_DEVICETREE}"
	[ -z "$default_dtb" ] && default_dtb="${MACHINE}.dtb"
	install -d ${D}/boot/dt-overlays
	for DTB_FILE in `ls *.dtbo`; do
		mv ${D}/boot/devicetree/${DTB_FILE} ${D}/boot/dt-overlays
	done

	find ${D}/boot/devicetree ! -name "${default_dtb}" -type f -exec rm -f {} +
}

devicetree_do_deploy() {
	cp -a ${D}/boot/* ${DEPLOYDIR}
}

FILES:${PN} += "boot/dt-overlays"

# --------------------------------------
# common include files

SRC_URI = " \
	${DTS_BASE} \
"

# --------------------------------------
# niot-e-nfl90-q2n16-n-revx boards

COMPATIBLE_MACHINE:niot-e-nfl90-q2n16-n-revx = ".*"
DTS_BASE:niot-e-nfl90-q2n16-n-revx += " \
	file://src/niot-e-nfl90-q2n16-n-revx.dtsi \
	file://src/pad_config.h \
"

SRC_URI:append:niot-e-nfl90-q2n16-n-rev1 = " \
	file://src/niot-e-nfl90-q2n16-n-rev1.dts \
"

# --------------------------------------
# netfield-iolink-edge-gw-revx boards

COMPATIBLE_MACHINE:netfield-iolink-edge-gw-revx = ".*"
DTS_BASE:netfield-iolink-edge-gw-revx += " \
	file://src/netfield-iolink-edge-gw-revx.dtsi \
	file://src/pad_config.h \
"

SRC_URI:append:netfield-iolink-edge-gw-rev1 = " \
	file://src/netfield-iolink-edge-gw-rev1.dts \
"

SRC_URI:append:netfield-iolink-edge-gw-rev2 = " \
	file://src/netfield-iolink-edge-gw-rev2.dts \
"

# --------------------------------------
# iotgate-rev1 demo board

COMPATIBLE_MACHINE:netfield-compact-x8m-revx = ".*"
DTS_BASE:netfield-compact-x8m-revx += " \
	file://src/netfield-compact-x8m-revx.dtsi \
"

SRC_URI:append:netfield-compact-x8m-rev1 = " \
	file://src/netfield-compact-x8m-rev1.dts \
	file://src/imx8gate-can.dtso \
	file://src/imx8gate-poed.dtso \
	file://src/imx8gate-ied-slot0-rs232.dtso \
	file://src/imx8gate-ied-slot1-rs232.dtso \
	file://src/imx8gate-ied-slot0-tpm.dtso \
	file://src/imx8gate-ied-slot1-tpm.dtso \
	file://src/imx8gate-ied-slot0-can.dtso \
	file://src/imx8gate-ied-slot1-can.dtso \
	file://src/imx8gate-uart1-rs485.dtso \
	file://src/imx8gate-uart1-rs232.dtso \
"
RDEPENDS:${PN}:netfield-compact-x8m-revx += "systemd-uart1-mode"
