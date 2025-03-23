#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif 

__attribute__((__noreturn__))
void abort(void);

int atoi(const char*);
char* itoa(int, char*, int);

#ifdef _cplusplus
}
#endif 
#endif // !_STDLIB_H
