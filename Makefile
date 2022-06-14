MAKE_DIR		:=		$(PWD)
BUILD_DIR		:=		$(MAKE_DIR)/build
CONFIG_DIR		:=		$(MAKE_DIR)/config
OUTPUT_DIR		:=		$(MAKE_DIR)/output

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	LD			:=		ld
	CC			:=		gcc
	GENISO		:=		geniso
endif
ifeq ($(UNAME_S), Darwin)
	LD			:=		i386-elf-ld
	CC			:=		i386-elf-gcc
	GENISO		:=		mkisofs
endif

export MAKE_DIR
export BUILD_DIR
export CONFIG_DIR
export LD
export CC
export GENISO

.PHONY: all
all:
	@$(MAKE) -C kernel -r -R --warn-undefined-variables 


.PHONY: run
run: all
	bochs -f $(CONFIG_DIR)/bochsrc.txt -q || echo "simulation ended"
	mv bochslog.txt $(OUTPUT_DIR)/
	mv com1.out $(OUTPUT_DIR)/

.PHONY: clean
clean:
	@$(MAKE) -C kernel -r -R --warn-undefined-variables clean

.PHONY: re
re: clean all

.PHONY: setup_macos
setup_macos:
	brew tap nativeos/i386-elf-toolchain
	brew install i386-elf-binutils i386-elf-gcc
	brew install cdrtools
	brew install nasm

.PHONY: setup_fedora
setup_fedora:
	dnf install nasm genisoimage

.PHONY: setup_debian
setup_debian:
	apt install nasm genisoimage
