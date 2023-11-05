/* multiboot header constants */
.set ALIGN, 1<<0 /* Align loaded modules on page boundries */
.set MEMINFO, 1<<1 /* Provide memory map*/
.set FLAGS, ALIGN | MEMINFO /* Multiboot flag field*/
.set MAGIC, 0x1BADB002 /*Lets bootloader find header*/
.set CHECKSUM, -(MAGIC + FLAGS) /* Proves we are multiboot via checksum*/

/*
 * Declare a multiboot header that marks the program as a kernel.
 * These are magic values that are docymented in the multiboot standard.
 * The bootloader will search fot this signature in the first 8KiB of
 * the kernel file, aligned at a 32-bit boundry.
 * The signature is in its own section so the header can be forced to be
 * within the first 8KiB of the kernel file.
 */
.section .multiboot 
.align 4
.long MAGIC 
.long FLAGS 
.long CHECKSUM 

/*
 * The multiboot standard doesn't define the value of the "stack pointer register,"
 * or esp, as it is up to the kernel to provide a stack.
 * This allocates room for a small stack through a few steps:
    * 1. Create a symbol at the bottom of the stack
    * 2. Allocate 16 KiB for the stack
    * 3. Create a symbol at the top of the stack.
 * The stack grows DOWN on x86.
 * Since the stack is in its own section*, it can be marked "nobits"
 * which means the kernel file is smaller since it doesn't contain
 * an uninitialized stack.
 * On x86, the stack must be 16-byte aligned** according to System V ABI standard
 * and de-facto extentions.
 * The compiler assumes the stack is properly aligned, so failure to align will
 * result in UB.
 */
.section bss 
.align 16 
.stack_bottom:
.skip 16384 
stack_top: 

/*
 * Our linker script specifies _start as te entry point to the kernel.
 * The bootloader will jump here once the kernel is loaded.
 * We won't return, since the bootloader would be gone at that point
 */
.section .text
.global _start
.type _start, @function
_start:
  /*
   * The bootloader loads us into 32-bit mode on x86 machines.
   * Interrupts and paging are disabled.
   * The multiboot standard defines our current processor state.
   * The kernel has full control over the CPU
   * The kernel can only use two things
      * 1. hardware features
      * 2. its own code
   * No printing is available (Unless we make it).
   * No security restrictions, safeguards, or debugging mechanisms.
   * We only have what the kernel provides.
   * ABSOLUTE POWER OVER THE MACHINE!!
   */

  /*
   * To setup a stack, make the esp register point to the top of the stack.
   * We have to do this in assembly since C wont even function without the stack.
   */
  mov $stack_top, %esp

  /*
   * This is a good point to initialize crucial processor state before our
   * high level kernel is entered.
   * It's best to minize early environment where crucial features are offline.
   * The processor is not fully initialized yet, therefore
   * features such as floating point instructions and instruction set extentions
   * are ALSO not initialized.
   * The GDT (global descriptor table) should be loaded here.
   * Paging should be enabled here.
   */

  /*
   * Enter our high level kernel.
   * the ABI (?) requres the stack is 16-byte aligned at the time of
   * the call instruction (which afterward will push the return pointer
   * of size 4 bytes).
   * The stack was 16-byte aligned above, and we have pushed a multiple of
   * 16 bytes to the stack (we have pushed zero bytes so far), so 
   * the alignment is preserved, and the call is well defined.
   */
  call kernel_main

  /*
   * If the system has nothing else to do, we will infinitely loop.
   * To do this, we must:
      * 1. Disable interrupts with `cli` 
      * (clear interrupt enable in eflags.)
      * They are already disabled by the bootloader, so we can skip this.
      * Keep in mind, we may have to do this later AND return from main 
      * (for some nonsensical reason)
      * 2. wait for the next interrupt to arrive with `hlt` (halt instruction)
      * Since interrupts are disabled, this locks up the computer (yay!)
      * 3. jump to the `hlt` instruction if it ever wakes up due to a
      * non-maskable interrupt occuring or due to a system management mode.
   */
    cli
1:  hlt
    jmp 1

/*
 * Set the size of _start to { current_location - (start of _start) }
 * This is useful for debugging or for call tracing later on.
 */
.size _start, . - _start

