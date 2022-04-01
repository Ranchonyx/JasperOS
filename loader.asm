extern kernel_start
extern kernel_end

extern kmain
global _start


MULTIBOOT_ALIGN            equ 1 << 0
MULTIBOOT_MEMINFO          equ 1 << 1
MULTIBOOT_HEADER_MAGIC     equ 0x1BADB002
MULTIBOOT_HEADER_FLAGS     equ MULTIBOOT_ALIGN | MULTIBOOT_MEMINFO
CHECKSUM                   equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
KSTACK_SIZE                equ 0x4000

section .multiboot
  dd MULTIBOOT_HEADER_MAGIC
  dd MULTIBOOT_HEADER_FLAGS
  dd CHECKSUM
  dd 00

section .text

_start:
align 4


  mov esp, stack_top         ; Set up stack for 4KB


  push kernel_start
  push kernel_end
  push ebx                   ; Push multiboot data structure
  push eax                   ; Push bootlader magic

  cli                        ; Clear Interrupts
  lgdt [gdt_desc]            ; Load Global Descriptor Table

  jmp 0x08:finish            ; Jump to kernel code segment
  finish:                    ; Prep
    mov ax, 0x0010
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    call kmain                  ; Call the C kernel's main function

halt:                        ; Halt the CPU if kmain ever returns
  cli
  hlt

section .gdt
gdt:
  gdt_null:
    dd 0x0
    dd 0x0
  gdt_code:
    dw 0xFFFF
    dw 0
    db 0
    db 10011010b
    db 11001111b
    db 0
  gdt_data:
    dw 0xFFFF
    dw 0
    db 0
    db 10010010b
    db 11001111b
    db 0
  gdt_end:

gdt_desc:
  dw gdt_end - gdt_null - 1
  dd gdt


section .bss
stack_bottom:
  resb KSTACK_SIZE
stack_top:
