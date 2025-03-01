ENTRY(_start)

SECTIONS
{
  /* 2M is safer than 1M bc of UEFI */
  . = 2M;

  /* Put the multiboot header */
  .text BLOCK(4K) : ALIGN(4K)
  {
    *(.multiboot)
    *(.text)
  }

  /* Read-only data */
  .rodata BLOCK(4K) : ALIGN(4K)
  {
    *(.rodata)
  }

  /* Read-write data */
  .data BLOCK(4K) : ALIGN(4K)
  {
    *(.data)
  }

  /* Uninitialized read-write data and stack*/
  .bss BLOCK(4K) : ALIGN(4K)
  {
    *(COMMON)
    *(.bss)
  }
}
