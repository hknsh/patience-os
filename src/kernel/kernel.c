/* PatienceOS - An operating system which only runs Solitaire
 * hknsh, 2025, MIT. */
#include <kernel/idt.h>
#include <kernel/tty.h>
#include <kernel/sh.h>
#include <kernel/gdt.h>
#include <libc/stdio.h>
#include <drivers/vga.h>
#include <libc/stdio.h>

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
  gdt_init();
  printf("kernel: gdt initialized.\n");
  idt_init();
  printf("kernel: idt initialized.\n");
}
