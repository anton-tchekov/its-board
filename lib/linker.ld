ENTRY(_start)
SECTIONS
{
    . = 0x08000000;
    .text :
    {
        *(.text*);
    }

    . = 0x20000000;
    .data :
    {
        *(.data*);
    }

    .bss :
    {
        *(.bss*);
    }
}
