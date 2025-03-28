#include <kernel/sh/hwinfo.h>
#include <kernel/memory.h>
#include <stdint.h>

void get_sys_info(struct sys_info* info) {
  uint32_t total_frames = page_frame_max - page_frame_min;
  uint32_t total_mem = total_frames * 4096;
  uint32_t used_mem = total_alloc * 4096;
  uint32_t free_mem = total_mem - used_mem;

  info->total_memory_kb = total_mem / 1024;
  info->used_memory_kb = used_mem / 1024;
  info->free_memory_kb = free_mem / 1024;

  info->total_allocated_pages = total_alloc;
  info->total_virtual_pages = mem_num_vpages;

  uint32_t ebx, edx, ecx;
  asm volatile(
    "mov $0, %%eax\n"
    "cpuid\n"
    : "=b"(ebx), "=d"(edx), "=c"(ecx)
    :
    : "eax"
  );

  *(uint32_t*)&info->cpu_vendor[0] = ebx;
  *(uint32_t*)&info->cpu_vendor[4] = edx;
  *(uint32_t*)&info->cpu_vendor[8] = ecx;
  info->cpu_vendor[12] = '\0';
}
