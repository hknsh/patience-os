#include <stdint.h>
#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif 

#define CEIL_DIVISION(a,b) (((a + b) - 1) / b)

__attribute__((__noreturn__))
void abort(void);

int atoi(const char*);
char* itoa(int, char*, int);
void utoa(unsigned int, char*);
void malloc_init(uint32_t);
void change_heap_size(int);
void* malloc(uint32_t size);

#ifdef _cplusplus
}
#endif 
#endif // !_STDLIB_H
