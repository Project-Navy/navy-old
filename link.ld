ENTRY(bootstrap)

SECTIONS
{
    . = 0xffffffff80100000;


    kernel_virtual_start = .;
    kernel_physical_start = . - 0xffffffff80000000;

    .stivalehdr ALIGN(4K) :
    {
        KEEP(*(.stivalehdr))
    }

    .stivale2hdr ALIGN(4K) :
    {
        KEEP(*(.stivale2hdr))
    }

    .text ALIGN(4K) :
    {
        KEEP(*(.text*))
    }

    .rodata ALIGN(4K) :
    {
        KEEP(*(.rodata*))
    }

    .data ALIGN(4K) :
    {
        KEEP(*(.data*))
    }

    .bss ALIGN(4K) :
    {
        KEEP(*(COMMON))
        KEEP(*(.bss*))
    }

    kernel_virtual_end = .;
    kernel_physical_end = . - 0xffffffff80000000;
}