MAKE_DIR		:=		$(PWD)
BUILD_DIR		:=		$(MAKE_DIR)/build
CONFIG_DIR		:=		$(MAKE_DIR)/config
OUTPUT_DIR		:=		$(MAKE_DIR)/output

export MAKE_DIR
export BUILD_DIR
export CONFIG_DIR

.PHONY: all
all:
	@$(MAKE) -C kernel -r -R --warn-undefined-variables 


.PHONY: run
run: all
	bochs -f $(CONFIG_DIR)/bochsrc.txt -q || echo "simulation ended"
	mv bochslog.txt $(OUTPUT_DIR)/

.PHONY: clean
clean:
	@$(MAKE) -C kernel -r -R --warn-undefined-variables clean

.PHONY: re
re: clean all
