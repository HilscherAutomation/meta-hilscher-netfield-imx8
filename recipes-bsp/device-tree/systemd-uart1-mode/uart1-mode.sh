#!/bin/sh

start() {
	mode=$1
	[ -z "$mode" -a -e /etc/default/uart1 ] && . /etc/default/uart1
	[ -z "$mode" ] && mode="rs485"

	if [ ! -e /boot/dt-overlays/imx8gate-uart1-$mode.dtbo ]; then
		echo "uart1: invalid mode ($mode)"
		stop
		return 1
	fi

	mkdir /sys/kernel/config/device-tree/overlays/uart1-$mode
	cat /boot/dt-overlays/imx8gate-uart1-$mode.dtbo > /sys/kernel/config/device-tree/overlays/uart1-$mode/dtbo
	echo "uart1: enabled ($mode)"

	return 0
}

stop() {
	for mode in rs485 rs232; do
		[ -e /sys/kernel/config/device-tree/overlays/uart1-$mode ] && rmdir /sys/kernel/config/device-tree/overlays/uart1-$mode
	done
	echo "uart1: disabled"

	return 0
}

cmd=$1 && shift 1

case $cmd in
	start)
		start "$@"
		;;
	stop)
		stop
		;;
	restart)
		stop
		start "$@"
		;;
esac

exit $?
