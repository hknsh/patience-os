#ifndef HWINFO_H
#define HWINFO_H

#include <stdint.h>

struct sys_info {
  uint32_t total_memory_kb;
  uint32_t used_memory_kb;
  uint32_t free_memory_kb;

  uint32_t total_allocated_pages;
  uint32_t total_virtual_pages;
  
  char cpu_vendor[13];
};

void get_sys_info(struct sys_info*);

#endif // !HWINFO_H
