#!/bin/sh

mount -t devtmpfs dev /dev
mount -t proc proc /proc
mount -t binfmt_misc binfmt_misc /proc/sys/fs/binfmt_misc
mount -t sysfs sysfs /sys
mount -t cgroup2 cgroup2 /sys/fs/cgroup
mount -t debugfs none /sys/kernel/debug

mkdir -p /dev/pts
mount -t devpts devpts /dev/pts

exec /sbin/getty -n -l /bin/sh 115200 /dev/hvc1
