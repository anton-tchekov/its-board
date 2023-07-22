ENTRY(_start)
SECTIONS
{
    . = 0x8000000;
    .start :
    {
        *(.start)
    }

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
