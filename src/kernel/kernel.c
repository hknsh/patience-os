/* PatienceOS - An operating system which only runs Solitaire
 * hknsh, 2025, MIT. */
#include <kernel/timer.h>
#include <kernel/idt.h>
#include <kernel/tty.h>
#include <kernel/sh.h>
#include <kernel/gdt.h>
#include <kernel/multiboot.h>
#include <kernel/memory.h>
#include <libc/stdlib.h>
#include <libc/stdio.h>
#include <libc/stdio.h>
#include <drivers/vga.h>
#include <drivers/keyboard.h>
#include <stdint.h>

#if defined(__linux__)
#error "Nuh uh, use a cross-compiler babe"
#endif

#if !defined(__i386__)
#error "Nuh uh sweetheart, you need a ix86-elf compiler"
#endif

void kernel_main(uint32_t magic, struct multiboot_info* boot_info);

void kernel_main(uint32_t magic, struct multiboot_info* boot_info)
{
  terminal_init();
  terminal_printstr("patienceOS/i386 (alpha) (kernel)\n");

  /* Init */
  terminal_setcolour(vga_entry_colour(VGA_COLOUR_LIGHT_GREEN, VGA_COLOUR_BLACK));
  gdt_init();
  printf("kernel: i386/gdt initialized.\n");
  idt_init();
  printf("kernel: i386/idt initialized.\n");
  timer_init();
  printf("kernel: i386/timer initialized.\n");
  init_keyboard_driver();
  printf("driver: drivers/keyboard initialized.\n");

  uint32_t mod1 = *(uint32_t*)(boot_info->mods_addr + 4);
  uint32_t physicalAllocStart = (mod1 + 0xFFF) & ~0xFFF;
  memory_init(boot_info->mem_upper * 1024, physicalAllocStart);
  malloc_init(0x1000);
  printf("kernel: i386/memory initialized.\n");

  printf("kernel: initializing sh.\n");
  terminal_lock_position();
  terminal_setcolour(vga_entry_colour(VGA_COLOUR_LIGHT_GREY, VGA_COLOUR_BLACK));

  shell_spawn();

  for(;;);
}
