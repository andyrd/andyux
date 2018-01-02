section .mbHeader

MODULEALIGN equ 1<<0
MEMINFO     equ 1<<1
FLAGS       equ MODULEALIGN | MEMINFO
MAGIC       equ 0x1BADB002
CHECKSUM    equ -(MAGIC + FLAGS)

bits 32

global mboot
extern code
extern bss
extern end
      
mboot:  
    dd MAGIC
    dd FLAGS
    dd CHECKSUM
    dd mboot
    dd code
    dd bss
    dd end
    dd start

global start
extern main

start:
   push ebx
   cli
   call main
   jmp $