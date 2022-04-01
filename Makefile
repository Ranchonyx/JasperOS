OBJECTS = loader.o kmain.o asmfunc.o vga.o ports.o sys.o string.o util.o IDT.o ISR.o exception_handler.o filesystem.o initrd.o pic.o acpi.o timer.o kheap.o paging.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -ffreestanding -nodefaultlibs -Wall -Wextra -c -Iinclude
LDFLAGS = -T link.ld -melf_i386 -Ttext=0x100000
AS = nasm
ASFLAGS = -f elf32

all: kernel.elf

kernel.elf: $(OBJECTS)
	@ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

jasper.iso: kernel.elf
	@cp kernel.elf iso/boot/kernel.elf
	@grub2-mkrescue -o jasper.iso iso

run: jasper.iso
	qemu-system-x86_64 -s -d guest_errors,int,mmu,cpu_reset -m 2G -enable-kvm -cpu host -soundhw pcspk -monitor stdio -D stdio -drive file=$<,format=raw,index=0,media=disk

%.o: %.c
	@$(CC) $(CFLAGS) $< -o $@ -g

%.o: %.asm
	@$(AS) $(ASFLAGS) $< -o $@ -g

clean:
	@rm -rf *.o kernel.elf jasper.iso
	@echo Done
