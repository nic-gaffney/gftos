MBALIGN   equ 1<<0  ; Align loaded modules on page boundries */
MBMEMINFO equ 1<<1  ; Provide memory map*/
MBFLAGS equ MBALIGN | MBMEMINFO ; Multiboot flag field*/
MAGIC equ 0x1BADB002 ;Lets bootloader find header*/
CHECKSUM equ -(MAGIC + MBFLAGS) ; Proves we are multiboot via checksum*/


section .multiboot
align 4
  dd MAGIC
  dd MBFLAGS
  dd CHECKSUM

section .bss
align 16
stack_bottom:
resb 16384
stack_top:

section .text
global _start:function (_start.end - _start)
_start:
  mov esp, stack_top

  extern get_gdtr
  call get_gdtr
  extern gdtr
  mov [gdtr], eax
  cli
  lgdt	[gdtr]


  call reloadSegments ; this motherfucker is what is causing so many issues

  [bits 32]
  extern kernel_main
  call kernel_main


	cli
.hang:	hlt
	jmp .hang
.end:


 reloadSegments:
    JMP   0x0008:.reload_CS ; should def define a CODESEG
.reload_CS:
   MOV   AX, 0x0010
   MOV   DS, AX
   MOV   ES, AX
   MOV   FS, AX
   MOV   GS, AX
   MOV   SS, AX
   RET
