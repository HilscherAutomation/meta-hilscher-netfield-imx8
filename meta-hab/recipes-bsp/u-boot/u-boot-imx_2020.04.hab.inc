#FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}_${PV}:"
FILESEXTRAPATHS_prepend := "${THISDIR}/u-boot-imx_2020.04:"

SRC_URI_append += " \
	file://enable_hab.cfg \
	file://0001-bootm-Disable-HAB-verification-for-fitImages.patch \
	file://0002-rsa-Selecting-RSA_SOFTWARE_EXP-by-default-when-RSA_F.patch \
	file://0003-Fix-HAB-issues-when-using-AUTOBOOT_STO_STR_SHA256.patch \
"

SRC_URI_append += " \
	file://mkimage_fit_atf.sh \
	file://print_fit_hab.sh \
"

do_compile_prepend() {
	cp ${WORKDIR}/mkimage_fit_atf.sh ${S}/arch/arm/mach-imx
	if [ -n "${UBOOT_CONFIG}" ]; then
		for config in ${UBOOT_MACHINE}; do
				ln -sf ${WORKDIR}/print_fit_hab.sh ${config}/print_fit_hab.sh
		done
	else
		ln -sf ${WORKDIR}/print_fit_hab.sh print_fit_hab.sh
	fi
}

PRINT_FIT_HAB_OFFSET ?= "0x60000"
do_compile_append() {
	if [ -n "${UBOOT_CONFIG}" ]; then
		for config in ${UBOOT_MACHINE}; do
			[ -z "${EXT_DTB}" ] && bbfatal "Invalid or missing virtual/dtb provider."
			ATF_LOAD_ADDR=${ATF_LOAD_ADDR} VERSION="v1" ${config}/print_fit_hab.sh ${PRINT_FIT_HAB_OFFSET} ${EXT_DTB} > print_fit_hab.log
		done
	else
		[ -z "${EXT_DTB}" ] && bbfatal "Invalid or missing virtual/dtb provider."
		ATF_LOAD_ADDR=${ATF_LOAD_ADDR} VERSION="v1" ./print_fit_hab.sh ${PRINT_FIT_HAB_OFFSET} ${EXT_DTB} > print_fit_hab.log
	fi
}

do_deploy_append() {
	if [ -n "${UBOOT_CONFIG}" ]; then
		for config in ${UBOOT_MACHINE}; do
			install -m 0777 ${B}/${config}/flash.log ${DEPLOYDIR}
			install -m 0777 ${B}/${config}/print_fit_hab.log ${DEPLOYDIR}
		done
	else
		install -m 0777 ${B}/flash.log ${DEPLOYDIR}
		install -m 0777 ${B}/print_fit_hab.log ${DEPLOYDIR}
	fi
}
