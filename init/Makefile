# SPDX-License-Identifier: GPL-3.0-or-later
# Copyright (C) Biswapriyo Nath
# This file is part of hvtool project.

BIN = init
BINIMG = initrd.img
CFLAGS = -D_GNU_SOURCE -pedantic -O2 -Wall
LDFLAGS =

ALPINE_DIR = alpine
ALPINE_TAR = alpine-minirootfs-3.21.3-x86_64.tar.gz
ALPINE_URL = https://dl-cdn.alpinelinux.org/alpine/v3.21/releases/x86_64/$(ALPINE_TAR)

all : $(BINIMG)

$(ALPINE_TAR) :
	mkdir -p $(ALPINE_DIR)
	wget $(ALPINE_URL)
	wget $(ALPINE_URL).sha256
	sha256sum -c $(ALPINE_TAR).sha256
	tar -xf $(ALPINE_TAR) -C $(ALPINE_DIR)

$(BIN) : init.c
	$(CC) -s $(CFLAGS) $^ $(LDFLAGS) -o $(ALPINE_DIR)/$@
	cp init_script.sh $(ALPINE_DIR)/

$(BINIMG) : $(ALPINE_TAR) $(BIN)
	cd $(ALPINE_DIR); find . -print0 | cpio -o -0 -H newc -F ../$@

clean :
	rm -rf $(ALPINE_DIR) $(ALPINE_TAR) $(ALPINE_TAR).sha256 $(BIN) $(BINIMG)
