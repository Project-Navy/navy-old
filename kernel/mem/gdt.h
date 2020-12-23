#ifndef _KERNEL_MEM_GDT_H_
#define _KERNEL_MEM_GDT_H_

#include <stdint.h>

typedef struct __attribute__((__packed__))
{
    uint16_t base0_15;
    uint16_t limit0_15;

    uint8_t base24_31;
    uint8_t flags: 4;
    uint8_t limit16_19: 4;
    uint8_t access;
    uint8_t base16_23;
} GdtEntry;

enum GDT_BIT_FIELD
{
    PRESENT    = 0b10000000,
    KERNEL     = 0b00000000,
    USER       = 0b01100000,
    CODE       = 0b00010000,
    DATA       = 0b00010000,
    TSS        = 0b00000000,
    READ_WRITE = 0b00000010,
};

void init_gdt(void);

#endif