#ifndef _LIBALLOC_H
#define _LIBALLOC_H


void *liballoc_malloc(size_t);
void *liballoc_realloc(void *, size_t);
void *liballoc_calloc(size_t, size_t);
void liballoc_free(void *);

#endif
