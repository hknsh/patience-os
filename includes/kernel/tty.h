#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>
#include <stddef.h>
#include <drivers/vga.h>

typedef struct {
  size_t row;
  size_t column;
  uint8_t colour;
  uint16_t* buffer;
} terminal_state_t;

void terminal_init(void);
void terminal_setcolour(uint8_t colour);
void terminal_putentryat(char c, uint8_t colour, size_t x, size_t y);
void terminal_putchar(char c);
void terminal_print(const char* data, size_t size);
void terminal_printstr(const char* data);

#endif // TERMINAL_H
