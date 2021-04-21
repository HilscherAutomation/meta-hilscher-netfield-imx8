SUMMARY = "Hilscher BSP device trees"
DESCRIPTION = "Hilscher BSP device trees from within layer."
SECTION = "bsp"

# the device trees from within the layer are licensed as MIT, kernel includes are GPL
LICENSE = "MIT & GPLv2"
LIC_FILES_CHKSUM = " \
	file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302 \
	file://${COMMON_LICENSE_DIR}/GPL-2.0;md5=801f80980d171dd6425610833a22dbe6 \
"

inherit devicetree

inherit dts-sign
# Setup public key patching into dts
DTS_SIGN_ENFORCE="${PLATFORM_SIGN}"
DTS_SIGN_KEY_DIR="${PLATFORM_KEYDIR}"
DTS_SIGN_KEY_NAME="${PLATFORM_KEYNAME}"

python do_apply_verification_keys_prepend() {
  if d.getVar('DTS_TO_SIGN') is None:
    dts_to_sign = d.getVar('KERNEL_DEVICETREE') or d.getVar('MACHINE')
    dts_to_sign = dts_to_sign.replace('.dtb','')+".dts"
    d.setVar('DTS_TO_SIGN', "${WORKDIR}/src/"+dts_to_sign)
}

do_unpack[vardeps] += "PLATFORM_SIGN PLATFORM_KEYDIR PLATFORM_KEYNAME"

S = "${WORKDIR}/src"

devicetree_do_install_append() {
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

FILES_${PN} += "boot/dt-overlays"

# --------------------------------------
# common include files

SRC_URI = " \
	${DTS_BASE} \
"

# --------------------------------------
# niot-e-nfl90-q2n16-n-revx boards

COMPATIBLE_MACHINE_niot-e-nfl90-q2n16-n-revx = ".*"
DTS_BASE_niot-e-nfl90-q2n16-n-revx += " \
	file://src/niot-e-nfl90-q2n16-n-revx.dts \
	file://src/pad_config.h \
"

SRC_URI_append_niot-e-nfl90-q2n16-n-rev1 += " \
	file://src/niot-e-nfl90-q2n16-n-rev1.dts \
"

# --------------------------------------
# netfield-iolink-edge-gw-revx boards

COMPATIBLE_MACHINE_netfield-iolink-edge-gw-revx = ".*"
DTS_BASE_netfield-iolink-edge-gw-revx += " \
        file://src/netfield-iolink-edge-gw-revx.dts \
        file://src/pad_config.h \
"

SRC_URI_append_netfield-iolink-edge-gw-rev1 += " \
        file://src/netfield-iolink-edge-gw-rev1.dts \
"

SRC_URI_append_netfield-iolink-edge-gw-rev2 += " \
        file://src/netfield-iolink-edge-gw-rev2.dts \
"
