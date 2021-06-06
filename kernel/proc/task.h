#ifndef _PROC_TASK_H_
#define _PROC_TASK_H_

#include <stdint.h>
#include <libk/range.h>

#define STACK_SIZE 0x1000

enum state
{
    RUNNING,
    JOINING,
    GONNADIE,
    DEPRESSED,
    SLEEPING,
    DEAD
};

typedef struct
{
    uint64_t pid;

    uintptr_t *address_space;
    uintptr_t stack_base;
    AddrRange stack;

    char name[64];
    void *args;

    uint64_t joinee_pid;
    uintptr_t return_value;
    uint64_t wakeup_tick;

    enum state state;

    void (*task)();
} Task;

void init_tasking(void);
uintptr_t *get_current_address_space(void);
bool is_userspace(void);
uint64_t sched(uint64_t);
uint64_t create_task(const char *name, void (*task)(), bool userspace);

#endif /* !_PROC_TASK_H_ */
