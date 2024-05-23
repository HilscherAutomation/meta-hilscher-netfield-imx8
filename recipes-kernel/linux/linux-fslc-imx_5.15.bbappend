# Include required compulab stuff
require compulab-bsp.inc

include linux-common_5.15.inc

# meta-freescale is still at an older commit for 5.15, so modify it
# NOTE: This will require that **we** need to update the recipe in future
SRCREV = "411c52448fdc0906f70c4585c7e05359c0b05c11"
SRCREV_meta = "a9112e1b2552a7b037b2f90699505e7c1e4d6a34"
SRCREV_FORMAT = "meta_${@d.getVar('SRCREV', True)[:10]}"

LINUX_VERSION="5.15.158"

# TODO: SMSC95xx patch fails to compile on newer kernels. Its' unclear if it is required
SRC_URI:remove = " file://0089-net-smsc95-Fix-phy-issue.patch"
