#include <libc/stdio.h>
#include <libc/string.h>
#include <libc/stdlib.h>
#include <kernel/tty.h>
#include <stdbool.h>
#include <stdarg.h>
#include <limits.h>

#define STDIN_BUFFER_SIZE 256

static bool print(const char* data, size_t length) {
  const unsigned char* bytes = (const unsigned char*) data;
  for (size_t i = 0; i < length; i++)
    if (putchar(bytes[i]) == EOF)
      return false;
  return true;
}

int printf(const char* restrict format, ...) {
  va_list args;
  va_start(args, format);

  int written = 0;
  char buffer[32]; // store converted numbers
  
  while (*format) {
    if (*format != '%') {
      if (!print(format, 1)) return -1;
      format++;
      written++;
      continue;
    }

    format++;
    switch (*format) {
      case 'c' : {
        char c = (char) va_arg(args, int);
        if (!print(&c, 1)) return -1;
        written++;
        break;
      }
      case 's' : {
        const char* str = va_arg(args, const char*);
        size_t len = strlen(str);
        if (!print(str, len)) return -1;
        written += len;
        break;
      }
      case 'd' : {
        int num = va_arg(args, int);
        itoa(num, buffer, 10); // convert to string
        size_t len = strlen(buffer);
        if (!print(buffer, len)) return -1;
        written += len;
        break;
      }
      case '%': {
        if (!print("%", 1)) return -1;
        written++;
        break;
      }
      default: {
        if (!print("%", 1)) return -1;
        if (!print(format, 1)) return -1;
        written += 2;
      }
    }
    format++;
  }
  va_end(args);
  return written;
}

int putchar(int ic) {
  char c = (char) ic;
  terminal_print(&c, sizeof(c));
  return ic;
}

char getchar(void) {
  static char stdin_buffer[STDIN_BUFFER_SIZE];
  static size_t buffer_index = 0;

  char c = 'h'; // TODO: Add keyboard chars here
  if (c == '\n' || buffer_index >= STDIN_BUFFER_SIZE - 1) {
    stdin_buffer[buffer_index] = '\0'; // finishes the string
    buffer_index = 0;
    return '\n';
  }
  stdin_buffer[buffer_index++] = c;
  return c;
}

char* gets(char* str) {
  char c;
  size_t i = 0;

  while ((c = getchar()) != '\n' && c != EOF)
    str[i++] = c;
  str[i] = '\0';
  return str;
}

int puts(const char* string) {
  return printf("%s\n", string);
}
