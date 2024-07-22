# Include required compulab stuff
require compulab-bsp.inc

include linux-common_5.4.inc

# meta-freescale is still at an older commit for 5.4, so modify it
# NOTE: This will require that **we** need to update the recipe in future
SRCREV = "f90fdee61a645ffde03469b79a1c1503ce225fd0"
SRCREV_meta = "318db1080fdd1e26567b6ba679310adad84c173a"
SRCREV_FORMAT = "meta_${@d.getVar('SRCREV', True)[:10]}"

# Update kernel via patch, as it is not yet available mainline
LINUX_VERSION = "5.4.280"
SRC_URI_append += "file://kernel-update-5.4.200-to-280.patch.gz"
addtask do_kernel_version_sanity_check after do_patch
