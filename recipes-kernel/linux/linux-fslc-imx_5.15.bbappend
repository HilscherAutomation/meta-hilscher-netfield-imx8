# Include required compulab stuff
require compulab-bsp.inc

include linux-common_5.15.inc

# meta-freescale is still at an older commit for 5.15, so modify it
# NOTE: This will require that **we** need to update the recipe in future
SRCREV = "81b79a06a058f65e64109b42646e528adc44d66e"
SRCREV_meta = "c07c75a1e9438e70a80615cac0f48eb91d8f34ea"
SRCREV_FORMAT = "meta_${@d.getVar('SRCREV', True)[:10]}"

LINUX_VERSION="5.15.148"
