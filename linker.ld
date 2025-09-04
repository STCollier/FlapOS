OUTPUT_FORMAT("binary")
ENTRY(_start)

SECTIONS
{
    . = 0x1000;

    .text : {
        *(.text.prologue)
        *(.text*)
    }

    .rodata : { *(.rodata*) }

    .data : { *(.data*) }

    .bss (NOLOAD) : {
        *(.bss*)
        *(COMMON)
    }

    end = .;
}
