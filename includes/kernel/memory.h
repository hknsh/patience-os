#ifndef MEMORY_H
#define MEMORY_H

#include <kernel/multiboot.h>
#include <stdint.h>

extern uint32_t initial_page_dir[1024];

#define KERNEL_START 0xC0000000
#define PAGE_FLAG_PRESENT (1 << 0)
#define PAGE_FLAG_WRITE (1 << 1)

void memory_init(uint32_t mem_high, uint32_t physical_alloc_start);
void pmm_init(uint32_t mem_low, uint32_t mem_high);
void invalidate(uint32_t vaddr);

#endif // !MEMORY_H
