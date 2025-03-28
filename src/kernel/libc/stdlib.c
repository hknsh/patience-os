#include <kernel/memory.h>
#include <libc/stdio.h>
#include <libc/stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Malloc Start */
static uint32_t heap_start;
static uint32_t heap_size;
static uint32_t threshold;
static uint32_t current_break;
static bool malloc_initialized = false;
/* Malloc End */

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

/* Converts an unsigned int into a string */
void utoa(unsigned int value, char* str) {
  char temp[10]; // 32 bit int
  int i = 0;

  if (value == 0) {
    str[0] = '0';
    str[1] = '\0';
    return;
  }

  while (value > 0) {
    temp[i++] = (value % 10) + '0';
    value /= 10;
  }

  for (int j = 0; j < i; j++) {
    str[j] = temp[i - j - 1];
  }

  str[i] = '\0';
}

/* Memory allocation */
void malloc_init(uint32_t initial_heap_size) {
  heap_start = KERNEL_MALLOC;
  heap_size = 0;
  threshold = 0;
  malloc_initialized = true;

  change_heap_size(initial_heap_size);
}

void change_heap_size(int new_size) {
  int old_page_top = CEIL_DIVISION(heap_size, 0x1000);
  int new_page_top = CEIL_DIVISION(new_size, 0x1000);

  int diff = new_page_top - old_page_top;

  for (int i = 0; i < diff; i++) {
    uint32_t phys = pmm_alloc_page_frame();
    mem_map_page(KERNEL_MALLOC + old_page_top * 0x1000 + i * 0x1000,
                 phys, PAGE_FLAG_WRITE);
  }
}

void* malloc(uint32_t size) {
  if (!malloc_initialized) return 0;
  size = (size + 7) & ~7; // align to 8 bytes
  if (current_break + size > heap_start + heap_size) {
    change_heap_size((current_break - heap_start) + size + 0x1000);
  }
  void* addr = (void*) current_break;
  current_break += size;
  return addr;
}
