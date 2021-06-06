#ifndef _LIBALLOC_COMPAT_H_
#define _LIBALLOC_COMPAT_H_

int liballoc_lock(void);
int liballoc_unlock(void);
void *liballoc_alloc(int);
int liballoc_free_(void *addr, int size);

#endif /* !_LIBALLOC_COMPAT_H_ */
