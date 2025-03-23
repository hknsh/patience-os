#include <kernel/idt.h>
#include <libc/stdio.h>
#include <libc/string.h>
#include <libc/ports.h>
#include "stdint.h"

struct idt_entry_struct idt_entries[256];
struct idt_ptr_struct idt_ptr;

extern void idt_flush(uint32_t);

void idt_init() {
  idt_ptr.limit = sizeof(struct idt_entry_struct) * 256 - 1;
  idt_ptr.base = (uint32_t)&idt_entries;

  memset(&idt_entries, 0, sizeof(struct idt_entry_struct) * 256);

  // Initialize PIC for interrupts
  // 0x20 commands and 0x21 data
  // 0xA0 commands and 0xA1 data
  outPortB(0x20, 0x11);
  outPortB(0xA0, 0x11);

  outPortB(0x21, 0x20);
  outPortB(0xA1, 0x28);

  outPortB(0x21, 0x04);
  outPortB(0xA1, 0x02);

  outPortB(0x21, 0x01);
  outPortB(0xA1, 0x01);

  outPortB(0x21, 0x0);
  outPortB(0xA1, 0x0);

  idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
  idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
  idt_set_gate(2, (uint32_t)isr2, 0x08, 0x8E);
  idt_set_gate(3, (uint32_t)isr3, 0x08, 0x8E);
  idt_set_gate(4, (uint32_t)isr4, 0x08, 0x8E); 
  idt_set_gate(5, (uint32_t)isr5, 0x08, 0x8E);
  idt_set_gate(6, (uint32_t)isr6, 0x08, 0x8E);
  idt_set_gate(7, (uint32_t)isr7, 0x08, 0x8E);
  idt_set_gate(8, (uint32_t)isr8, 0x08, 0x8E);
  idt_set_gate(9, (uint32_t)isr9, 0x08, 0x8E);
  idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
  idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
  idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
  idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
  idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
  idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
  idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
  idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E); 
  idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
  idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
  idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
  idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
  idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
  idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
  idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
  idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
  idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
  idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
  idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
  idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
  idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
  idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);

  idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
  idt_set_gate(32, (uint32_t)irq1, 0x08, 0x8E);
  idt_set_gate(32, (uint32_t)irq2, 0x08, 0x8E);
  idt_set_gate(32, (uint32_t)irq3, 0x08, 0x8E);
  idt_set_gate(32, (uint32_t)irq4, 0x08, 0x8E);
  idt_set_gate(32, (uint32_t)irq5, 0x08, 0x8E);
  idt_set_gate(32, (uint32_t)irq6, 0x08, 0x8E);
  idt_set_gate(32, (uint32_t)irq7, 0x08, 0x8E);
  idt_set_gate(32, (uint32_t)irq8, 0x08, 0x8E);
  idt_set_gate(32, (uint32_t)irq9, 0x08, 0x8E);
  idt_set_gate(32, (uint32_t)irq10, 0x08, 0x8E);
  idt_set_gate(32, (uint32_t)irq11, 0x08, 0x8E);
  idt_set_gate(32, (uint32_t)irq12, 0x08, 0x8E);
  idt_set_gate(32, (uint32_t)irq13, 0x08, 0x8E);
  idt_set_gate(32, (uint32_t)irq14, 0x08, 0x8E);
  idt_set_gate(32, (uint32_t)irq15, 0x08, 0x8E);

  idt_set_gate(128, (uint32_t)isr128, 0x08, 0x8E); // System calls
  idt_set_gate(177, (uint32_t)isr177, 0x08, 0x8E); // System calls

  idt_flush((uint32_t)&idt_ptr);
}

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
  idt_entries[num].base_low = base & 0xFFFF;
  idt_entries[num].base_high = (base >> 16) & 0xFFFF;
  idt_entries[num].sel = sel;
  idt_entries[num].always0 = 0;
  idt_entries[num].flags = flags | 0x60;
}

const char* exception_messages[] = {
  "kernel: exception: division by zero",
  "kernel: exception: debug",
  "kernel: exception: non maskable interrupt",
  "kernel: exception: breakpoint",
  "kernel: exception: into detected overflow",
  "kernel: exception: out of bounds",
  "kernel: exception: invalid opcode",
  "kernel: exception: no coprocessor",
  "kernel: exception: double fault",
  "kernel: exception: coprocessor segment overrun",
  "kernel: exception: bad TSS",
  "kernel: exception: segment not present",
  "kernel: exception: stack fault",
  "kernel: exception: general protection fault",
  "kernel: exception: page fault",
  "kernel: exception: unknown interrupt",
  "kernel: exception: coprocessor fault",
  "kernel: exception: alignment fault",
  "kernel: exception: machine check",
  "kernel: exception: reserved",
  "kernel: exception: reserved",
  "kernel: exception: reserved",
  "kernel: exception: reserved",
  "kernel: exception: reserved",
  "kernel: exception: reserved",
  "kernel: exception: reserved",
  "kernel: exception: reserved",
  "kernel: exception: reserved",
  "kernel: exception: reserved",
  "kernel: exception: reserved",
  "kernel: exception: reserved"
};

void isr_handler(struct InterruptRegisters* regs) {
  if (regs->int_no < 32) {
    printf(exception_messages[regs->int_no]);
    printf("\n");
    printf("panic! kernel is sad. halted.\n");
    for(;;);
  }
}

void *irq_routines[16] = {
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0
};

void irq_install_handler(int irq, void (*handler)(struct InterruptRegisters *r)) {
  irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq) {
  irq_routines[irq] = 0;
}

void irq_handler(struct InterruptRegisters *regs) {
  void (*handler)(struct InterruptRegisters *regs);
  handler = irq_routines[regs->int_no - 32];

  if (handler) {
    handler(regs);
  }

  if (regs->int_no >= 40) {
    outPortB(0xA0, 0x20);
  }

  outPortB(0x20, 0x20);
}
