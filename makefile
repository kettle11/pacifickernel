arch ?= x86_64
kernel := build/kernel-$(arch).bin
iso := build/os-$(arch).iso

linker_script := src/arch/$(arch)/linker.ld
grub_cfg := src/arch/$(arch)/grub.cfg
assembly_source_files := $(wildcard src/arch/$(arch)/*.asm)
assembly_object_files := $(patsubst src/arch/$(arch)/%.asm, \
	build/arch/$(arch)/%.o, $(assembly_source_files))

c_source_files := $(wildcard src/arch/$(arch)/*.c)
c_object_files := $(patsubst src/arch/$(arch)/%.c, \
	build/arch/$(arch)/%.o, $(c_source_files))

CCOMPILER = $(HOME)/opt/cross/bin/x86_64-elf-gcc
LINKERFLAGS = -Wl,-n -ffreestanding -O2 -nostdlib -lgcc
CFLAGS = -c -mno-red-zone -ffreestanding -O2 -Wall -Wextra


.PHONY: all clean run iso

all: $(kernel)

clean:
	@rm -r build

run: $(iso)
	@qemu-system-x86_64 -cdrom $(iso)

iso: $(iso)

checkmultiboot:
	@if grub-file --is-x86-multiboot $(kernel); then\
  		echo MULTIBOOT CHECK FOR $(kernel): FAIL!;\
	else\
  		echo MULTIBOOT CHECK FOR $(kernel): SUCCESS!;\
  	fi

$(iso): $(kernel) $(grub_cfg)
	@mkdir -p build/isofiles/boot/grub
	@cp $(kernel) build/isofiles/boot/kernel.bin
	@cp $(grub_cfg) build/isofiles/boot/grub
	@grub-mkrescue -o $(iso) build/isofiles 2> /dev/null
	@rm -r build/isofiles

$(kernel): $(assembly_object_files) $(c_object_files) $(linker_script) 
	@$(CCOMPILER) $(LINKERFLAGS) -T $(linker_script) -o $(kernel) $(assembly_object_files) $(c_object_files) 
	@if grub-file --is-x86-multiboot $(kernel); then\
  		echo MULTIBOOT CHECK FOR $(kernel): FAIL!;\
	else\
  		echo MULTIBOOT CHECK FOR $(kernel): SUCCESS!;\
  	fi
# compile assembly files
build/arch/$(arch)/%.o: src/arch/$(arch)/%.asm
	@mkdir -p $(shell dirname $@)
	@nasm -felf64 $< -o $@

# compile C files
build/arch/$(arch)/%.o: src/arch/$(arch)/%.c
	@$(CCOMPILER) $(CFLAGS) -I src/arch/$(arch)/include/ $< -o $@