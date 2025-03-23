#ifndef TIMER_H
#define TIMER_H

#include <kernel/idt.h>

void timer_init();
void on_irq0(struct InterruptRegisters *regs);

#endif // !TIMER_H
