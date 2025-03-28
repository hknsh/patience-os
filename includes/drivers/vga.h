#ifndef VGA_H
#define VGA_H

#include <stdint.h>
#include <stddef.h>

enum vga_colour
{
  VGA_COLOUR_BLACK = 0,
  VGA_COLOUR_BLUE = 1,
  VGA_COLOUR_GREEN = 2,
  VGA_COLOUR_CYAN = 3,
  VGA_COLOUR_RED = 4,
  VGA_COLOUR_MAGENTA = 5,
  VGA_COLOUR_BROWN = 6,
  VGA_COLOUR_LIGHT_GREY = 7,
  VGA_COLOUR_DARK_GREY = 8,
  VGA_COLOUR_LIGHT_BLUE = 9,
  VGA_COLOUR_LIGHT_GREEN = 10,
  VGA_COLOUR_LIGHT_CYAN = 11,
  VGA_COLOUR_LIGHT_RED = 12,
  VGA_COLOUR_LIGHT_MAGENTA = 13,
  VGA_COLOUR_LIGHT_BROWN = 14,
  VGA_COLOUR_WHITE = 15
}; 

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VIDEO_MEMORY = (uint16_t*) 0xC00B8000;

uint8_t vga_entry_colour(enum vga_colour fg, enum vga_colour bg);
uint16_t vga_entry(unsigned char uc, uint8_t colour);

#endif // VGA_H
