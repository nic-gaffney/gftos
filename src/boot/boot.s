MBALIGN   equ 1<<0  ; Align loaded modules on page boundries */
MBMEMINFO equ 1<<1  ; Provide memory map*/
MBFLAGS equ MBALIGN | MBMEMINFO ; Multiboot flag field*/
MAGIC equ 0x1BADB002 ;Lets bootloader find header*/
CHECKSUM equ -(MAGIC + MBFLAGS) ; Proves we are multiboot via checksum*/
CODESEG equ 0x08
DATASEG equ 0x10

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

global gdtr
gdtr:
  dw 0
  dd 0

global idtr
idtr:
  dw 0
  dd 0

global page_ptr
page_ptr:
  dd 0

global _start:function (_start.end - _start)
_start:
  mov esp, stack_top

  extern get_gdtr
  call get_gdtr
  cli
  lgdt [gdtr]
  mov eax, cr0
  or al, 1
  mov cr0, eax

  call reloadSegments

  extern get_idtr
  call get_idtr
  lidt [idtr]
  sti
  xchg bx, bx

  extern genDirs
  extern page_dir
  call genDirs
  mov eax, page_dir
  mov cr3, eax
  mov eax, cr0
  or eax, 0x80000001
  mov cr0, eax

  [bits 32]
  extern kernel_main
  call kernel_main
  ; BOCHS



	cli
.hang:	hlt
	jmp .hang
.end:

reloadSegments:
  JMP   CODESEG:.reload_CS ; should def define a CODESEG
.reload_CS:
   MOV   AX, DATASEG
   MOV   DS, AX
   MOV   ES, AX ; TODO: Setup proper extra segment
   MOV   FS, AX ; TODO: Setup proper General purpose segments
   MOV   GS, AX ; ...
   MOV   SS, AX ; TODO: Setup proper stack segment
   RET
