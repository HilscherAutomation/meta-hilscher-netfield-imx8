SUMMARY = "HAB signing tools"
HOMEPAGE = "www.hilscher.com"
LICENSE = "CLOSED"
LIC_FILES_CHKSUM = ""

DEPENDS += "openssl-native"
DEPENDS += "libconfig"
DEPENDS += "byacc-native flex-native"

SRC_URI = " \
	file://cst-${PV}.tgz \
"

S = "${WORKDIR}/cst-${PV}"
B = "${S}"

do_compile() {
	BITNESS=$(getconf LONG_BIT)

	cd ${B}/code/cst

	# Compiling cst and libfrontend.a ...
	oe_runmake OSTYPE="linux${BITNESS}" rel_bin
	mkdir -p release/linux${BITNESS}/lib
	cp code/obj.linux${BITNESS}/libfrontend.a release/linux${BITNESS}/lib
	cp -r release/linux${BITNESS} ${B}

	cd ${B}/code/back_end-engine/src

	# Compiling libbackend.a ...
	oe_runmake

	# Compiling cst ...
	make all
}

do_install() {
	cd ${B}/code/back_end-engine/src
	install -d ${D}/${sbindir}
	install -m 744 cst ${D}/${sbindir}/cst-hsm
}

BBCLASSEXTEND = "native"
PACKAGES = "${PN}"
FILES_${PN} += "${sbindir}"
