CC				:=		gcc
CC_FLAGS		:=		-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs
CC_FLAGS		+=		-Wall -Wextra -Werror
CC_FLAGS		+=		-I include -I srcs

LD				:=		ld
LD_FLAGS		:=		-T $(CONFIG_DIR)/link.ld -melf_i386

NASM			:=		nasm
NASM_FLAGS		:=		-f elf32

GENISO			:=		genisoimage
GENISO_FLAGS	:=		-R -b boot/grub/stage2_eltorino -no-emul-boot	\
						-boot-load-size 4 -A os -input-charset utf8		\
						-quiet -boot-info-table

SRCS_NAME		:=		kmain.c io/ports.c io/vga.c io/serial.c boot/boot.s
SRCS_DIR		:=		srcs
SRCS			:=		$(addprefix $(SRCS_DIR)/, $(SRCS_NAME))

OBJECTS_NAME	:=		$(addsuffix .o, $(basename $(SRCS_NAME)))
OBJECTS_DIR		:=		$(BUILD_DIR)
OBJECTS			:=		$(addprefix $(OBJECTS_DIR)/, $(OBJECTS_NAME))

OUT_DIRECTORIES	:=		$(sort $(addprefix $(OBJECTS_DIR)/, $(dir $(SRCS_NAME))))

ISO_NAME		:=		os.iso
ISO				:=		$(addprefix $(OBJECTS_DIR)/, $(ISO_NAME))

KERNEL_NAME		:=		kernel.elf
KERNEL			:=		$(addprefix $(OBJECTS_DIR)/, $(KERNEL_NAME))

.PHONY: all
all: $(OUT_DIRECTORIES) iso

.PHONY: iso
iso: $(ISO)

$(ISO): iso/boot/kernel.elf iso/boot/grub/menu.lst iso/boot/grub/stage2_eltorino
	$(GENISO) $(GENISO_FLAGS) -o $(ISO) iso

iso/boot/kernel.elf: $(KERNEL)
	cp $(KERNEL) iso/boot/

$(KERNEL): $(OBJECTS) $(CONFIG_DIR)/link.ld
	$(LD) $(LD_FLAGS) $(OBJECTS) -o $(KERNEL)

$(OBJECTS_DIR)/%.o: $(SRCS_DIR)/%.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(OBJECTS_DIR)/%.o: $(SRCS_DIR)/%.s
	$(NASM) $(NASM_FLAGS) $< -o $@

$(OUT_DIRECTORIES):
	mkdir -p $^

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(KERNEL) iso/boot/kernel.elf $(ISO)

.PHONY: re
re: clean all
