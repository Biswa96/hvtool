#!/bin/sh

mount -t devtmpfs dev /dev
mount -t proc proc /proc
mount -t sysfs sysfs /sys

exec /sbin/getty -n -l /bin/sh 115200 /dev/hvc1
