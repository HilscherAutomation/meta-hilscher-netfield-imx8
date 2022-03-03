FILESEXTRAPATHS_prepend := "${THISDIR}/${BPN}:"

RDEPENDS_${PN}_append_netfield-compact-x8m-rev1 += "i2c-tools device-tree"
