#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>

#define EOF (-1)

#ifdef _cplusplus
extern "C" {
#endif 

int printf(const char* __restrict, ...);
int putchar(int);
int puts(const char*);

#ifdef _cplusplus
}
#endif 
#endif // !_STDIO_H
