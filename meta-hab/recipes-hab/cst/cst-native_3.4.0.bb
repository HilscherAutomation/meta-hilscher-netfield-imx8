SUMMARY = "HAB signing tools"
HOMEPAGE = "www.hilscher.com"
LICENSE = "BSD-3-Clause & OpenSSL & HIDAPI"
LIC_FILES_CHKSUM = " \
    file://LICENSE.bsd3;md5=14aba05f9fa6c25527297c8aac95fcf6 \
    file://LICENSE.hidapi;md5=e0ea014f523f64f0adb13409055ee59e \
    file://LICENSE.openssl;md5=3441526b1df5cc01d812c7dfc218cea6 \
"
NO_GENERIC_LICENSE[HIDAPI] = "LICENSE.hidapi"

inherit native

DEPENDS += "openssl-native"
DEPENDS += "byacc-native flex-native"
DEPENDS += "u-boot-tools-native"

SRC_URI = " \
	file://cst-${PV}.tgz \
	file://use_external_toolchain.patch \
	file://fix_warnings.patch \
"

S = "${WORKDIR}/cst-${PV}"
B = "${S}/code/obj.linux64"

EXTRA_OEMAKE = "OSTYPE=linux64 CC='${CC} ${CFLAGS}' LD='${CC}' LDOPTIONS='${LDFLAGS}'"

do_compile() {
	oe_runmake
}

do_install() {
	install -d ${D}/${sbindir}
	install -m 744 ${B}/cst ${D}/${sbindir}/cst
}

PACKAGES = "${PN}"
FILES:${PN} += "${sbindir}"
