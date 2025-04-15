ENTRY(Reset_Handler)

MEMORY
{
    FLASH(rx)  : ORIGIN = 0x08000000, LENGTH = 64K;
    RAM(rwx)   : ORIGIN = 0x20000000, LENGTH = 20K;
}

/* Initial stack pointer set to top of RAM */
_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH

    .text :
    {
        . = ALIGN(4);
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
    } > FLASH

    /* Initialized data section */
    _sidata = LOADADDR(.data);
    .data : AT(_sidata)
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM

    /* Zero-initialized data section */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > RAM

    .stack (COPY) :
    {
        . = ALIGN(8);
    }
}
