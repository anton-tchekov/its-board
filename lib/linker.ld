ENTRY(_start)
SECTIONS
{
    . = 0x08000000;
    .text :
    {
        *(.text*);
    }

    .rodata :
    {
        *(.rodata*);
    }

    __data_end = .;
    .data :
    {
        *(.data*);
    }
    __data_end = .;

    . = 0x20000000;
    __bss_start = .;
    .bss :
    {
        *(.bss*);
    }
    __bss_end = .;
}
