#include <drivers/vga.h>

/* Hardware text mode colour constants */
uint8_t vga_entry_colour(enum vga_colour fg, enum vga_colour bg)
{
  return fg | bg << 4;
}

uint16_t vga_entry(unsigned char uc, uint8_t colour)
{
  return (uint16_t)uc | (uint16_t)colour << 8;
}

