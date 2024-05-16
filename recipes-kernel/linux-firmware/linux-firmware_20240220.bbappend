
# firmware for wifi card ax200
PACKAGES =+ " ${PN}-iwlwifi-cc-a0-50 "

LICENSE_${PN}-iwlwifi-cc-a0-50  = "Firmware-iwlwifi_firmware"
FILES_${PN}-iwlwifi-cc-a0-50    = "${nonarch_base_libdir}/firmware/iwlwifi-cc-a0-50.ucode"
RDEPENDS_${PN}-iwlwifi-cc-a0-50 = "${PN}-iwlwifi-license"
