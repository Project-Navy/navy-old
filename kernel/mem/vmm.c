#include "vmm.h"
#include "pmm.h"
#include "kernel/macro.h"
#include <libk/debug.h>
#include <libk/range.h>
#include <string.h>

extern uintptr_t kernel_physical_start;
extern uintptr_t kernel_physical_end;

static void *kernel_plm4;

void
init_vmm(void)
{
    module("VMM");

    AddrRange kernel_range_physical =
    {
        .base = (uintptr_t) &kernel_physical_start,
        .length = (uintptr_t) &kernel_physical_end - (uintptr_t) &kernel_physical_start,
    };

    kernel_plm4 = alloc_pmm(1);
    memset(kernel_plm4, 0, PAGE_SIZE);

    log_debug(INFO, "Kernel physical position: 0x%x", kernel_range_physical.base);
    log_debug(INFO, "Kernel required pages: %d", kernel_range_physical.length / PAGE_SIZE);

    align_range(&kernel_range_physical);
    set_used_pmm(kernel_range_physical); 
}
