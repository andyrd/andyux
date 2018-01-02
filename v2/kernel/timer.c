#include "timer.h"
#include "io.h"
#include "isr.h"
#include "print.h"

const dword pitHz = 1193180;
const word pitCommand = 0x43;
const word pitChannel0 = 0x40;

const dword timerHz = 100;

static void timerIsr(registers regs)
{
}

//http://wiki.osdev.org/PIT
void initTimer()
{
   registerInterruptHandler(irq0, &timerIsr);   

   dword divisor = pitHz/timerHz;

   outb(pitCommand, 0x36);

   byte low = (byte)(divisor & 0xFF);
   byte high = (byte)((divisor >> 8) & 0xFF);

   outb(pitChannel0, low);
   outb(pitChannel0, high);

   irqClearMask(irq0);
   enableInterrupts();   
}
