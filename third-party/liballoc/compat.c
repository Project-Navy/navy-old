#include "kernel/mem/vmm.h"
#include "kernel/macro.h"
#include <libk/debug.h>

static size_t lock_queue = 0;

int
liballoc_lock(void)
{
    __asm__ ("cli"); /* Maybe I should make a better lock ? */
    lock_queue++;

    return 0;
}

int
liballoc_unlock(void)
{
    lock_queue--;
    assert(lock_queue >= 0);
    
    if (lock_queue == 0)
    {
        __asm__ ("sti");
    }

    return 0;
}

void *
liballoc_alloc(int pages)
{
    /*uintptr_t *address_space = get_current_address_space();
    uintptr_t addr;
    
    alloc_vmm(address_space, pages * PAGE_SIZE, &addr, is_userspace());

    return (void *) addr;*/

    __unused(pages);

    return NULL;
}

int
liballoc_free_(void *addr, int size)
{
    /*AddrRange range =
    {
        .base = (uintptr_t) addr,
        .length = (uintptr_t) addr + size
    };

    free_vmm(get_current_address_space(), range);*/

    __unused(addr);
    __unused(size);
    
    return 0;
}
