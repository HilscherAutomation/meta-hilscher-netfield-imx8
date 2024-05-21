SUMMARY = "HAB signing"
HOMEPAGE = "www.hilscher.com"
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = ""

PACKAGE_ARCH = "${MACHINE_ARCH}"
DEPENDS = " \
	cst-native \
	openssl-native \
	gnutls-native \
	xxd-native \
"

SRC_URI = " \
	file://src \
	file://boot-recovery.cmd \
"

S = "${WORKDIR}/src"
B = "${WORKDIR}/src"

inherit sign-wrapper

# These files are provided by virtual/bootloader and used by the signing process.
FILESEXTRAPATHS:prepend := "${DEPLOY_DIR_IMAGE}:"
SRC_URI += " \
	file://flash.bin;subdir=${B}/hab \
	file://flash.log;subdir=${B}/hab \
	file://print_fit_hab.log;subdir=${B}/hab \
"
do_fetch[depends] += "virtual/bootloader:do_deploy"
do_fetch[cleandirs] += "${B}/hab"
do_fetch[vardeps] += "SIGN_WRAPPER_PKCS11_REMOTE SIGN_WRAPPER_KEYS_SHA HAB_SRK_TABLE HAB_CSF_KEY HAB_IMG_KEY"

do_configure[vardeps] += "SIGN_WRAPPER_PKCS11_REMOTE HAB_SRK_TABLE HAB_CSF_KEY HAB_IMG_KEY"
do_configure() {
	setup_sign_wrapper_env

	HAB_SRK_TABLE="${HAB_SRK_TABLE}"
	HAB_CSF_KEY="${HAB_CSF_KEY}"
	HAB_IMG_KEY="${HAB_IMG_KEY}"

	if [ "${SIGN_WRAPPER_MODE}" = "file" ]; then
		# Prepend path as signing tool requires full path
		HAB_SRK_TABLE="${SIGN_WRAPPER_KEY_SRC}/$HAB_SRK_TABLE"
		HAB_CSF_KEY="${SIGN_WRAPPER_KEY_SRC}/$HAB_CSF_KEY"
		HAB_IMG_KEY="${SIGN_WRAPPER_KEY_SRC}/$HAB_IMG_KEY"
	fi

	sed -i "s,###_HAB_SRK_TABLE_###,$HAB_SRK_TABLE,g" hab/*.in
	sed -i "s,###_HAB_CSF_KEY_###,$HAB_CSF_KEY,g" hab/*.in
	sed -i "s,###_HAB_IMG_KEY_###,$HAB_IMG_KEY,g" hab/*.in
}

do_compile () {
	setup_sign_wrapper_env

	if [ "${SIGN_WRAPPER_MODE}" = "pkcs11" ]; then
		export CST="cst -b pkcs11"
	fi

	bbwarn "A HAB signing process will be done."
	oe_runmake clean
	oe_runmake srk-fuse
	oe_runmake imx-boot

	# Build 2.3 compatible USB recovery script
	uboot-mkimage -A arm64 -c none -T script -d ${WORKDIR}/boot-recovery.cmd ${B}/hab/boot.scr
	oe_runmake boot-scr
}

inherit deploy
do_deploy() {
	install -d ${DEPLOYDIR}/soc-fuses
	install -m 0644 ${B}/hab/signed/srk-fuse/*.bin ${DEPLOYDIR}/soc-fuses
	install -m 0744 ${B}/hab/signed/srk-fuse/*.sh ${DEPLOYDIR}/soc-fuses
	install -m 0644 ${B}/hab/signed/u-boot/flash.bin.signed ${DEPLOYDIR}

	# NOTE: If HAB is activated, IMX_BOOT_FILE is already extended by the machine configuration with .signed.
	ln -sf flash.bin.signed ${DEPLOYDIR}/${IMX_BOOT_FILE}

	# Deploy 2.3 compatible USB recovery script
	install -m 0644 ${B}/hab/signed/boot-scr/boot.scr.signed ${DEPLOYDIR}/boot-2.3-recovery.scr
}
addtask deploy after do_compile

# NOTE: Allow an empty package to enable adding this to MACHINE_ESSENTIAL_EXTRA _ * variables.
ALLOW_EMPTY:${PN} = "1"

inherit hilscher-deploy

hd_path = "${HDEPLOY_PATH_EXTRAS}/soc-fuses"

do_hilscher_deploy() {
        cp -r ${DEPLOYDIR}/soc-fuses/* "${hd_path}/"
}
do_hilscher_deploy[cleandirs] = "${hd_path}/"
addtask hilscher_deploy before do_build after do_deploy
