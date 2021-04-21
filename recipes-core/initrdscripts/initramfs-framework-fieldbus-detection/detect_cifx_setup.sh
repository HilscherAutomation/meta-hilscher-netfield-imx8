#!/bin/sh

if [ "$1" = "init" ]; then
    modprobe uio_netx
    modprobe spidev
else
    modprobe -r uio_netx
fi
