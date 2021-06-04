#include "vmm.h"
#include "pmm.h"
#include <libk/debug.h>

void
init_vmm(void)
{
    module("VMM");
    void *plm4 = alloc_pmm(1);
    (void) plm4;
}
