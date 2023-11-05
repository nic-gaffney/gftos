/* The bootloader witll look at this image and start execution at
 * the symbol desugbated as ENTRY
 *
 * In this case, it is _start in boot.s
 */
ENTRY (_start)

/* Tell where the sections of the object files (.o) will be put in the
 * final image
 */
SECTIONS
{
  /* Put sections at 1MiB, the conventional place for kernels to be 
   * loaded at by the bootloader. */
  . = 1M;

  /* First, put the multiboot header. It is required to be put 
   * VERY early in the image or the bootloader wont recognize the file 
   * format. Next, we will put the .text section*/
  .text BLOCK(4K) : ALIGN(4K)
  {
    *(.multiboot)
    *(.text)
  }
  /* Read only data */
  .rodata BLOCK(4K) : ALIGN(4K)
  {
    *(.rodata)
  }
  /* Read write data (initialized) */
  .data BLOCK(4K) : ALIGN(4K)
  {
    *(.data)
  }
  /* Read write data (uninitialized) and stack */
  .bss BLOCK(4K) : ALIGN(4K)
  {
    *(COMMON)
    *(.bss)
  }

  /* The compiler may produce other sections, but they will be put in
   * a section with the same name by default. Add stuff here as needed */
}
