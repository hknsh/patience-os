#include "libc/ports.h"
#include <drivers/vga.h>
#include <kernel/tty.h>
#include <libc/string.h>
#include <stdint.h>

static terminal_state_t terminal; 

void terminal_init(void)
{
  terminal.row = 0;
  terminal.column = 0;
  terminal.colour = vga_entry_colour(VGA_COLOUR_LIGHT_GREY, VGA_COLOUR_BLACK);
  terminal.buffer = VIDEO_MEMORY; 
  for (size_t y = 0; y < VGA_HEIGHT; y++)
  {
    for (size_t x = 0; x < VGA_WIDTH; x++)
    {
      const size_t index = y * VGA_WIDTH + x;
      terminal.buffer[index] = vga_entry(' ', terminal.colour);
    }
  }
  terminal_update_cursor(terminal.row, terminal.column);
}

void terminal_setcolour(uint8_t colour)
{
  terminal.colour = colour;
}

void terminal_putentryat(char c, uint8_t colour, size_t x, size_t y)
{
  const size_t index = y * VGA_WIDTH + x;
  terminal.buffer[index] = vga_entry(c, colour);
}

void terminal_scroll(void) {
  // Moves everything one line above
  for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      size_t to_index = y * VGA_WIDTH + x;
      size_t from_index = (y + 1) * VGA_WIDTH + x;
      terminal.buffer[to_index] = terminal.buffer[from_index];
    }
  }

  // Cleans the last line
  for (size_t x = 0; x < VGA_WIDTH; x++) {
    size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
    terminal.buffer[index] = vga_entry(' ', terminal.colour);
  }

  terminal_update_cursor(terminal.row, terminal.column);
}

void terminal_putchar(char c) {
  unsigned char uc = c;

  if (uc == '\n') {
    terminal.column = 0;
    if (++terminal.row == VGA_HEIGHT) {
      terminal_scroll();
      terminal.row = VGA_HEIGHT - 1;
    }
    terminal_update_cursor(terminal.row, terminal.column);
    return;
  }

  if (uc == '\b') {
    if (terminal.column > 0) {
      if (!(terminal.row == terminal.protected_row && terminal.column <= terminal.protected_column)) {
        terminal.column--;
        terminal_putentryat(' ', terminal.colour, terminal.column, terminal.row);
        terminal_update_cursor(terminal.row, terminal.column);
      }
    } else if (terminal.row > terminal.protected_row) {
      terminal.row--;
      terminal.column = VGA_WIDTH - 1;

      do {
        terminal.column--;
        uint16_t entry = terminal.buffer[terminal.row * VGA_WIDTH + terminal.column];
        char ch = (char)(entry & 0xFF);
        if (ch != ' ') {
          break;
        }
      } while (terminal.column > 0);

      if (!(terminal.row < terminal.protected_row ||
      (terminal.row == terminal.protected_row && terminal.column < terminal.protected_column))) {
        terminal_putentryat(' ', terminal.colour, terminal.column, terminal.row);
        terminal_update_cursor(terminal.row, terminal.column);
      }
    }
    return;
  }

  terminal_putentryat(uc, terminal.colour, terminal.column, terminal.row);

  if (++terminal.column == VGA_WIDTH) {
    terminal.column = 0;
    if (++terminal.row == VGA_HEIGHT) {
      terminal_scroll();
      terminal.row = VGA_HEIGHT - 1;
    }
  }
  
  terminal_update_cursor(terminal.row, terminal.column);
}

void terminal_print(const char *data, size_t size)
{
  for (size_t i = 0; i < size; i++)
    terminal_putchar(data[i]);
}

void terminal_printstr(const char *data)
{
  terminal_print(data, strlen(data));
}

void terminal_update_cursor(int row, int col) {
  uint16_t pos = row * VGA_WIDTH + col;
  outPortB(0x3D4, 0x0F); // cursor location low byte
  outPortB(0x3D5, (uint8_t)(pos & 0xFF));
  outPortB(0x3D4, 0x0E); // cursor location high byte
  outPortB(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void terminal_lock_position() {
  terminal.protected_row = terminal.row;
  terminal.protected_column = terminal.column;
}
