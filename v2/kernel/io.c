#include "io.h"

void outb(word port, byte val)
{
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

byte inb(word port)
{
   byte result;
   asm volatile("inb %1, %0" : "=a"(result) : "Nd"(port));

   return result;
}

void io_wait()
{
   asm volatile("outb %%al, $0x80" : : "a"(0));
}
