#include "isr.h"
#include "memory.h"
#include "io.h"
#include "idt.h"
#include "print.h"

const byte irq0 = 32;
const byte irq1 = 33;
const byte irq2 = 34;
const byte irq3 = 35;
const byte irq4 = 36;
const byte irq5 = 37;
const byte irq6 = 38;
const byte irq7 = 39;
const byte irq8 = 40;
const byte irq9 = 41;
const byte irq10 = 42;
const byte irq11 = 43;
const byte irq12 = 44;
const byte irq13 = 45;
const byte irq14 = 46;
const byte irq15 = 47;

isr registeredHandlers[256];

void cpuIsrRouter(registers regs)
{
   if(registeredHandlers[regs.intNumber])
   {
      registeredHandlers[regs.intNumber](regs);
   }
}

void irqIsrRouter(registers regs)
{   
   //Send an end of interrupt to the master if it came in on the master or the master and the slave
   //if it came in on the slave.
   if(regs.intNumber >= irq8)
   {
      outb(pic2Command, picEoi);
   }

   outb(pic1Command, picEoi);

   if(registeredHandlers[regs.intNumber])
   {
      registeredHandlers[regs.intNumber](regs);
   }
}

void clearInterruptHandlers()
{
   memset(&registeredHandlers, 0, sizeof(registeredHandlers));
}

void registerInterruptHandler(byte interrupt, isr handler)
{
   registeredHandlers[interrupt] = handler;
}

void irqSetMask(byte interrupt)
{
   word port;   

   if(interrupt < irq8)
   {
      port = pic1Data;
      interrupt -= irq0;
   }
   else
   {
      port = pic2Data;
      interrupt -= irq8;
   }

   byte value = inb(port) & (1 << interrupt);
   outb(port, value);
}

void irqClearMask(byte interrupt)
{
   word port;   

   if(interrupt < irq8)
   {
      port = pic1Data;
      interrupt -= irq0;
   }
   else
   {
      port = pic2Data;
      interrupt -= irq8;
   }

   byte value = inb(port) & ~(1 << interrupt);
   outb(port, value);
}

void disableInterrupts()
{
   asm volatile("cli");
}

void enableInterrupts()
{
   asm volatile("sti");
}
