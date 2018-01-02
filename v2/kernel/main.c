#include "screen.h"
#include "print.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "timer.h"

void main(void* mbh)
{
   initGdt();
   initIdt();

   clearInterruptHandlers(); 

   hideCursor();
   clearScreen();

   printf("Welcome to Andyux v2.0");
   
   initTimer();
}
