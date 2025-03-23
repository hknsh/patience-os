#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <kernel/idt.h>

// global keyboard receiver, default sh.
extern void (*keyboard_receiver)(char c);

void init_keyboard_driver();
void handle_keyboard(struct InterruptRegisters *regs);

#endif // !KEYBOARD_H
