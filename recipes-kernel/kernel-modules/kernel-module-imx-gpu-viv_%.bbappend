# Signature is stripped for external modules, thus fix it here
inherit sign-wrapper

INHIBIT_PACKAGE_STRIP="1"
EXTRA_OEMAKE += "INSTALL_MOD_STRIP=1"
