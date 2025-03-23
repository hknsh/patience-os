/* PatienceOS - An operating system which only runs Solitaire
 * hknsh, 2025, MIT. */
#include <kernel/timer.h>
#include <kernel/idt.h>
#include <kernel/tty.h>
#include <kernel/sh.h>
#include <kernel/gdt.h>
#include <libc/stdio.h>
#include <libc/stdio.h>
#include <drivers/vga.h>
#include <drivers/keyboard.h>

#if defined(__linux__)
#error "Nuh uh, use a cross-compiler babe"
#endif

#if !defined(__i386__)
#error "Nuh uh sweetheart, you need a ix86-elf compiler"
#endif


void kernel_main(void)
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
  printf("kernel: initializing sh.\n");
  terminal_lock_position();
  terminal_setcolour(vga_entry_colour(VGA_COLOUR_LIGHT_GREY, VGA_COLOUR_BLACK));

  shell_spawn();

  for(;;);
}
