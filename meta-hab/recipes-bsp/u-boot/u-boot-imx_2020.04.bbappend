HAB_SUPPORT = "${@bb.utils.contains('MACHINE_FEATURES', 'hab', 'u-boot-imx_2020.04.hab.inc', 'dummy.inc', d)}"

include ${HAB_SUPPORT}
