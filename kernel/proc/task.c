#include <stdint.h>
#include <string.h>
#include <libk/debug.h>
#include <stdbool.h>

#include "task.h"
#include "kernel/mem/vmm.h"
#include "kernel/int/interrupt.h"
#include "third-party/liballoc/liballoc.h"

static int64_t pid = -1;
static uint64_t newPid = 0;
static uint64_t lastPid;

static Task *tasks;

static uintptr_t *current_addrspace;
static bool userspace = false;

uintptr_t *
get_current_address_space(void)
{
    return current_addrspace;
}

bool
is_userspace(void)
{
    return userspace;
}

uint64_t
create_task(const char *name, void (*task)(), bool userspace)
{
    module_push();

    module("SCHEDULER");

    log_debug(INFO, "OK");
    
    uintptr_t addr;
    uintptr_t *plm4;
    Task *new_task;
    InterruptStackFrame *frame;
    
    __asm__ ("cli");
    assert(strlen(name) >= 64);

    plm4 = alloc_plm4();
    current_addrspace = plm4;
    
    new_task = (Task *) liballoc_malloc(sizeof(Task));

    alloc_vmm(plm4, STACK_SIZE / PAGE_SIZE, &addr, userspace);

    new_task->stack = (AddrRange) {
        .base = addr,
        .length = STACK_SIZE
    };

    new_task->joinee_pid = 0;
    new_task->pid = newPid;
    new_task->stack_base = addr + STACK_SIZE;
    new_task->address_space = plm4;

    frame = (InterruptStackFrame *) (addr - sizeof(InterruptStackFrame));
    frame->cs = 0x08;
    frame->rflags = 0x202;
    frame->rip = (uintptr_t) task;

    frame->r15 = 0;
    frame->r14 = 0;
    frame->r13 = 0;
    frame->r12 = 0;
    frame->r11 = 0;
    frame->r10 = 0;
    frame->r9 = 0;
    frame->r8 = 0;
    frame->rax = 0;
    frame->rbx = 0;
    frame->rcx = 0;
    frame->rdx = 0;
    frame->rbp = 0;
    frame->rsi = 0;
    frame->rdi = 0;

    new_task->state = RUNNING;
    strcpy(new_task->name, name);
    log_debug(INFO, "Task %d (%s) created !", new_task->pid, new_task->name);

    
    lastPid = newPid++;
    tasks[lastPid] = *new_task;

    tasks = (Task *) liballoc_realloc(tasks, sizeof(Task) * newPid);

    __asm__("sti");
    
    module_pop();

    return new_task->pid;
}

void
init_tasking(void)
{
    module("SCHEDULER");

    current_addrspace = get_krnl_addrspace();
    tasks = (Task *) liballoc_malloc(sizeof(Task));

    pid = create_task("System", NULL, false);
}

static void
update_task_state(void)
{
    size_t i;
    Task joinee;

    for (i = 0; i < lastPid; i++)
    {
        if (tasks[i].state == JOINING)
        {
            joinee = tasks[tasks[i].joinee_pid];

            if (joinee.state == DEPRESSED)
            {
                tasks[i].return_value = joinee.return_value;
                joinee.state = GONNADIE;
                tasks[i].state = RUNNING;
            }
        }

        if (tasks[i].state == SLEEPING)
        {
            if (tasks[i].wakeup_tick <= fetch_tick())
            {
                tasks[i].state = RUNNING;
            }
        }

    }
}

uint64_t
sched(uint64_t rsp)
{
    module_push();
    
    module("SCHEDULER");
    log_debug(INFO, "tick");
    
    if (pid == -1)
    {
        return rsp;
    }
    
    update_task_state();
    tasks[pid].stack_base = rsp;

    do
    {
        pid++;

        if ((uint64_t) pid >= lastPid)
        {
            pid = 0;
        }
    } while (tasks[pid].state != RUNNING);


    log_debug(INFO, "Switching to %s", tasks[pid].name);
    
    module_pop();
    return tasks[pid].stack_base;
}
