HAB_SUPPORT = "${@bb.utils.contains('MACHINE_FEATURES', 'hab', 'u-boot-compulab_2021.04.hab.inc', 'dummy.inc', d)}"

include ${HAB_SUPPORT}
