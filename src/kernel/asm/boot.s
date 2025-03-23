; Constants for the multiboot header
%define ALIGN     1 << 0
%define MEMINFO   1 << 1
%define FLAGS     (ALIGN | MEMINFO)
%define MAGIC     0x1BADB002
%define CHECKSUM  -(MAGIC + FLAGS)

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .bss
align 16
stack_bottom:
    resb 16384        ; 16 KiB
stack_top:

section .text
global _start
extern kernel_main
_start:
    ; Set up the stack
    mov esp, stack_top

    ; Call kernel_main (will be linked from C code)
    call kernel_main

    ; Halt the CPU forever
    cli
.hang:
    hlt
    jmp .hang
