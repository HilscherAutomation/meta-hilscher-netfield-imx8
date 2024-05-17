LICENSE="GPL-2.0-only"

include u-boot-common_2021.04.inc

# NXP repositories were moved to GitHub
SRC_URI:remove = "git://source.codeaurora.org/external/imx/uboot-imx.git;protocol=https;branch=${SRCBRANCH}"
SRC_URI:append = " git://github.com/nxp-imx/uboot-imx;protocol=https;branch=${SRCBRANCH}"

COMPATIBLE_MACHINE = "(netfield-compact-x8m-rev1|netfield-iolink-edge-gw-rev2|niot-e-nfl90-q2n16-n-rev1)"

# We build flash.bin ourself
inherit imx-boot-container

DEPENDS += " \
    ${IMX_EXTRA_FIRMWARE} \
    imx-atf \
    ${@bb.utils.contains('MACHINE_FEATURES', 'optee', 'optee-os', '', d)} \
"

do_resolve_and_populate_binaries[depends] += " \
    ${@' '.join('%s:do_deploy' % r for r in '${IMX_EXTRA_FIRMWARE}'.split() )} \
    imx-atf:do_deploy \
    ${@bb.utils.contains('MACHINE_FEATURES', 'optee', 'optee-os:do_deploy', '', d)} \
"
