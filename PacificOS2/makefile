arch ?= x86_64
kernel := build/kernel-$(arch).so
efi := build/BOOTX64.efi
img := build/os-$(arch).img

c_source_files := $(wildcard source/*.c)
c_object_files := $(patsubst source/%.c, \
	build/%.o, $(c_source_files))

CCOMPILER = clang
LINKER = ld
LINKERFLAGS = -nostdlib -T efi/gnuefi/elf_x86_64_efi.lds -shared -Bsymbolic -L efi/x86_64/lib -L efi/gnuefi 
CFLAGS = -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -DHAVE_USE_MS_ABI -c
EFIINCLUDE = -Iefi/inc -Iefi/inc/x86_64 -Iefi/inc/protocol

.PHONY: all clean run img

all: $(kernel)

clean:
	@rm -r build

run: $(img)
	@qemu-system-x86_64 -display sdl -vga std -L emulator/ -bios OVMF-pure-efi.fd -usb -usbdevice disk::$(img) -vga std -d guest_errors

img: $(img)

$(img): $(efi)
	@mkdir -p build/
	@dd if=/dev/zero of=$(img) bs=1k count=1440
	@mformat -i $(img) -f 1440 ::
	@mmd -i $(img) ::/EFI
	@mmd -i $(img) ::/EFI/BOOT
	@mcopy -i $(img) $(efi) ::/EFI/BOOT

$(efi): $(kernel)
	@objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym  -j .rel -j .rela -j .reloc --target=efi-app-x86_64 $(kernel) $(efi)

$(kernel): $(c_object_files)
	@echo "LINKING ---------------------------"
	@$(LINKER) $(LINKERFLAGS) -o $(kernel) $(c_object_files) efi/gnuefi/crt0-efi-x86_64.o -lefi -lgnuefi

# compile C files
build/%.o: source/%.c
	@echo "BUILDING ---------------------------"

	@mkdir -p $(shell dirname $@)
	@$(CCOMPILER) $(EFIINCLUDE) $(CFLAGS) -I include/ $< -o $@