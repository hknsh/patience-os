; Constants for the multiboot header
%define MBOOT_ALIGN     1 << 0
%define MBOOT_MEMINFO   1 << 1
%define MBOOT_USE_GFX   0 ; Enable when start graphical
%define MBOOT_FLAGS     (MBOOT_ALIGN | MBOOT_MEMINFO | MBOOT_USE_GFX)
%define MBOOT_MAGIC     0x1BADB002
%define MBOOT_CHECKSUM  -(MBOOT_MAGIC + MBOOT_FLAGS)

section .multiboot
align 4
    dd MBOOT_MAGIC
    dd MBOOT_FLAGS
    dd MBOOT_CHECKSUM
    dd 0, 0, 0, 0, 0
    dd 0
    dd 800
    dd 600
    dd 32

section .bss
align 16
stack_bottom:
    resb 16384 * 8      ; 128 KiB
stack_top:

section .boot

global _start
_start:
  mov ecx, (initial_page_dir - 0xC0000000)
  mov cr3, ecx
  
  mov ecx, cr4
  or ecx, 0x10
  mov cr4, ecx

  mov ecx, cr0
  or ecx, 0x80000000
  mov cr0, ecx

  jmp higher_half

section .text
higher_half:
  mov esp, stack_top
  push ebx
  push eax
  xor ebp, ebp
  extern kernel_main
  call kernel_main
halt:
  hlt
  JMP halt

section .data
align 4096
global initial_page_dir
initial_page_dir:
  dd (0 << 22) | 0x183
  times 768-1 dd 0

  dd (0 << 22) | 0x183
  dd (1 << 22) | 0x183
  dd (2 << 22) | 0x183
  dd (3 << 22) | 0x183
  times 256-4 dd 0
