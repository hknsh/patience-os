#include <libc/ports.h>
#include <libc/stdio.h>
#include <kernel/timer.h>
#include <stdint.h>

uint64_t ticks;
const uint32_t freq = 100;

void on_irq0(struct InterruptRegisters *regs) {
  ticks += 1;
}

void timer_init() {
  ticks = 0;
  irq_install_handler(0, &on_irq0);

  // 1.1931816666 MHz -> 119318.16666 Hz
  uint32_t divisor = 1193180/freq; // <- freq make it work faster 

  // 0x43 -> Mode/Command register
  // 0x36 -> 0011 0110 <- more details in osdev wiki
  outPortB(0x43,0x36);
  outPortB(0x40,(uint8_t)(divisor & 0xFF));
  outPortB(0x40,(uint8_t)((divisor >> 8) & 0xFF));
}

