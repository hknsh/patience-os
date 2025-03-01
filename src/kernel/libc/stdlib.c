#include <libc/stdio.h>
#include <libc/stdlib.h>

__attribute__((__noreturn__))
void abort(void) {
  #if defined (__is_libk)
  // TODO: add a better kernel panic
  printf("kernel killed: panic: abort()\n");
    asm volatile("hlt");
  #else
    // TODO: kill the process as if by SIGABRT
    printf("abort()\n");
  #endif
    while(1) { }
    __builtin_unreachable();
}
