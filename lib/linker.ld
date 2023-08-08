ENTRY(main)

MEMORY
{
	RAM (xrw)     : ORIGIN = 0x20000000, LENGTH =  256K
	FLASH (rx)    : ORIGIN = 0x08000000, LENGTH = 2048K
}

SECTIONS
{
	.text :
	{
		. = ALIGN(4);
		__text_start = .;
		*(.text*);
		__text_end = .;
	} > FLASH

	.rodata :
	{
		. = ALIGN(4);
		__rodata_start = .;
		*(.rodata*);
		__rodata_end = .;
	} > FLASH

	.data :
	{
		. = ALIGN(4);
		__data_start = .;
		*(.data*);
		__data_end = .;
	} > RAM AT > FLASH

	.bss :
	{
		. = ALIGN(4);
		__bss_start = .;
		*(.bss*)
		*(COMMON)
		__bss_end = .;
	} > RAM
}
