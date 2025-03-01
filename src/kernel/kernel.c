/* PatienceOS - An operating system which only runs Solitaire
 * hknsh, 2025, MIT. */

/* Shit is broken in UEFI, need to do some pixel buffer shite or something like that */

#include <stdbool.h>

#include <libc/string.h>
#include <kernel/tty.h>
#include <drivers/vga.h>

#if defined(__linux__)
#error "Nuh uh, use a cross-compiler babe"
#endif

#if !defined(__i386__)
#error "Nuh uh sweetheart, you need a ix86-elf compiler"
#endif

void kernel_main(void)
{
  terminal_init();
  terminal_setcolour(vga_entry_colour(VGA_COLOUR_LIGHT_BLUE, VGA_COLOUR_BLACK));
  terminal_printstr("PatienceOS kernel prototype\n");
}
