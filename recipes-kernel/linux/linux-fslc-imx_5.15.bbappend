# Include required compulab stuff
require compulab-bsp.inc
require cve-exclusions.inc

include linux-common_5.15.inc

# meta-freescale is still at an older commit for 5.15, so modify it
# NOTE: This will require that **we** need to update the recipe in future
SRCREV = "411c52448fdc0906f70c4585c7e05359c0b05c11"
SRCREV_meta = "a9112e1b2552a7b037b2f90699505e7c1e4d6a34"
SRCREV_FORMAT = "meta_${@d.getVar('SRCREV', True)[:10]}"

LINUX_VERSION="5.15.162"
LOCALVERSION=""
# Update kernel via patch, as it is not yet available mainline
SRC_URI:append = " file://linux-5.15.158-to-162.patch"
addtask do_kernel_version_sanity_check after do_patch

# TODO: SMSC95xx patch fails to compile on newer kernels. Its' unclear if it is required
SRC_URI:remove = " file://0089-net-smsc95-Fix-phy-issue.patch"

# PREEMPT-RT
# ----------
RT_PATCHES = " \
    file://patch-5.15.158-rt76.patch \
    file://enable_preempt_rt.cfg \
"
PV .= "${@bb.utils.contains('MACHINE_FEATURES', 'preempt-rt', '-rt', '', d)}"

SRC_URI:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'preempt-rt', d.getVar('RT_PATCHES', True), '', d)}"

LINUX_KERNEL_TYPE = "${@bb.utils.contains('MACHINE_FEATURES', 'preempt-rt', 'preempt-rt', 'standard', d)}"

do_kernel_configme:append() {
    if [ "${@bb.utils.contains('MACHINE_FEATURES', 'preempt-rt', 'rt', '', d)}" = "rt" ]; then
        sed -i -e 's/CONFIG_PREEMPT=y/# CONFIG_PREEMPT is not set/' \
               -e 's/# CONFIG_PREEMPT_RT is not set/CONFIG_PREEMPT_RT=y/' ${B}/.config
    fi
}
# ----------
