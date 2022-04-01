ENTRY(_start)

SECTIONS {

	. = 0x100000;
	kernel_start = .;

	.multiboot(4K) :
	{
		*(.multiboot)
	}

	.text ALIGN(4K) :
	{
		*(.text)
	}

	.rodata ALIGN(4K) :
	{
		*(.rodata)
    *(.gdt)
	}

	.data ALIGN(4K) :
	{
		*(.data)
	}

	.bss ALIGN(4K) :
	{
		*(COMMON)
		*(.bss)
		*(.stack)
	}

    kernel_end = .;
}
