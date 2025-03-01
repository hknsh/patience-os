/* Constants for the multiboot header */
.set ALIGN,     1<<0              /* align loaded modules on page boundaries */
.set MEMINFO,   1<<1              /* provide memory map */
.set FLAGS,     ALIGN | MEMINFO   /* Multiboot 'flag' field */
.set MAGIC,     0x1BADB002        /* 'magic number' to find the header */
.set CHECKSUM,  -(MAGIC + FLAGS)  /* checksum of above, to prove we are multiboot */

/* Declare a multiboot header that marks the program as a kernel */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* Defines stack pointer register (esp) */
.section .bss
.align 16
stack_bottom:
  .skip 16384 # 16 KiB
stack_top:

/* Entry point */
.section .text
.global _start
.type _start, @function
_start:
  /* 32-bit protected mode, everything is disabled. Good luck creating everything from scratch mate :+1: */
  /* Setup stack by setting the esp register to the top of the stack*/
  mov $stack_top, %esp

  /* GDT, Paging, etc should be loaded here */
  
  /* GTFO of assembly */
  call kernel_main

   /* If the system has nothing more to do, put it in a infinite loop */
   cli
1: hlt
   jmp 1b

.size _start, . - _start
