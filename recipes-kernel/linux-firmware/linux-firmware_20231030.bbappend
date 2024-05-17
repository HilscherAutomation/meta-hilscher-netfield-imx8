# firmware for wifi card ax200
PACKAGES =+ " ${PN}-ax200"

LICENSE_${PN}-ax200  = "Firmware-iwlwifi_firmware"
FILES_${PN}-ax200    = "${nonarch_base_libdir}/firmware/iwlwifi-cc-a0-*.ucode"
RDEPENDS_${PN}-ax200 = "${PN}-iwlwifi-license"
