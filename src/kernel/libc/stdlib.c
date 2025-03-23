#include <libc/stdio.h>
#include <libc/stdlib.h>

__attribute__((noreturn))
void abort(void) {
#if defined(is_libk)
    printf("Kernel panic: abort()\n");
    asm volatile("cli");
    asm volatile("hlt");
#else
    printf("abort()\n");
    while (1) { }
#endif
    __builtin_unreachable();
}

/* Converts a string to an int. */
int atoi(const char* str) {
  int result = 0;
  int sign = 1;

  while (*str == ' ')
    str++;

  if (*str == '-' || *str == '+') {
    if (*str == '-')
      sign = -1;
    str++;
  }

  while (*str >= '0' && *str <= '9') {
    result = result * 10 + (*str - '0');
    str++;
  }

  return result * sign;
}

/* Converts an int to a string */
char* itoa(int value, char* str, int base) {
  char* ptr = str;
  char* start = str;
  int negative = 0;

  if (value < 0 && base == 10) {
    negative = 1;
    value = -value;
  }

  do {
    int digit = value % base;
    *ptr++ = (digit > 9) ? (digit - 10) + 'a' : digit + '0';
    value /= base;
  } while (value > 0);

  if (negative)
    *ptr++ = '-';

  *ptr = '\0';

  for (char *left = start, *right = ptr - 1; left < right; left++, right--) {
    char temp = *left;
    *left = *right;
    *right = temp;
  }

  return str;
}
