
# try to load recovery image via USB
# since this script is located on USB stick we don't need to start/reset usb
loadaddr=0x70000000
if load usb 0:1 ${loadaddr} Image; then
	setenv bootargs "$bootargs console=${console} root=LABEL=RECOVERY rootwait rw rootdelay=1 roottimeout=10 loglevel=4";
	bootm ${loadaddr} ${loadaddr} ${loadaddr}
fi
