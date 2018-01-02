#include "idt.h"
#include "memory.h"
#include "io.h"

const word pic1Command = 0x20;
const word pic1Data    = 0x21;
const word pic2Command = 0xA0;
const word pic2Data    = 0xA1;
const byte picInit     = 0x11;
const byte pic8086     = 0x01;
const byte picEoi      = 0x20;

idtEntry idtEntries[256];
idtDescriptor idtDesc;

extern void writeIdt(dword descriptorAddress);
extern void cpuIsr0();
extern void cpuIsr1();
extern void cpuIsr2();
extern void cpuIsr3();
extern void cpuIsr4();
extern void cpuIsr5();
extern void cpuIsr6();
extern void cpuIsr7();
extern void cpuIsr8();
extern void cpuIsr9();
extern void cpuIsr10();
extern void cpuIsr11();
extern void cpuIsr12();
extern void cpuIsr13();
extern void cpuIsr14();
extern void cpuIsr15();
extern void cpuIsr16();
extern void cpuIsr17();
extern void cpuIsr18();

extern void irqIsr0();
extern void irqIsr1();
extern void irqIsr2();
extern void irqIsr3();
extern void irqIsr4();
extern void irqIsr5();
extern void irqIsr6();
extern void irqIsr7();
extern void irqIsr8();
extern void irqIsr9();
extern void irqIsr10();
extern void irqIsr11();
extern void irqIsr12();
extern void irqIsr13();
extern void irqIsr14();
extern void irqIsr15();

static void setIdtEntry(int num, dword base, word segmentSelector, byte flags)
{
   idtEntries[num].baseLow = base & 0xFFFF;
   idtEntries[num].baseHigh = (base >> 16) & 0xFFFF;
   
   idtEntries[num].segmentSelector = segmentSelector;
   idtEntries[num].alwaysZero = 0;
   idtEntries[num].flags = flags;
}

//0x08 indicates the kernel code segment
//for the flag values, http://wiki.osdev.org/IDT
static void setCpuIsrEntries()
{
   setIdtEntry(0, (dword)cpuIsr0, 0x08, 0x8E);
   setIdtEntry(1, (dword)cpuIsr1, 0x08, 0x8E);
   setIdtEntry(2, (dword)cpuIsr2, 0x08, 0x8E);
   setIdtEntry(3, (dword)cpuIsr3, 0x08, 0x8E);
   setIdtEntry(4, (dword)cpuIsr4, 0x08, 0x8E);
   setIdtEntry(5, (dword)cpuIsr5, 0x08, 0x8E);
   setIdtEntry(6, (dword)cpuIsr6, 0x08, 0x8E);
   setIdtEntry(7, (dword)cpuIsr7, 0x08, 0x8E);
   setIdtEntry(8, (dword)cpuIsr8, 0x08, 0x8E);
   setIdtEntry(9, (dword)cpuIsr9, 0x08, 0x8E);
   setIdtEntry(10, (dword)cpuIsr10, 0x08, 0x8E);
   setIdtEntry(11, (dword)cpuIsr11, 0x08, 0x8E);
   setIdtEntry(12, (dword)cpuIsr12, 0x08, 0x8E);
   setIdtEntry(13, (dword)cpuIsr13, 0x08, 0x8E);
   setIdtEntry(14, (dword)cpuIsr14, 0x08, 0x8E);
   setIdtEntry(15, (dword)cpuIsr15, 0x08, 0x8E);
   setIdtEntry(16, (dword)cpuIsr16, 0x08, 0x8E);
   setIdtEntry(17, (dword)cpuIsr17, 0x08, 0x8E);
   setIdtEntry(18, (dword)cpuIsr18, 0x08, 0x8E);
}

static void setIrqIsrEntries()
{
   setIdtEntry(32, (dword)irqIsr0, 0x08, 0x8E);
   setIdtEntry(33, (dword)irqIsr1, 0x08, 0x8E);
   setIdtEntry(34, (dword)irqIsr2, 0x08, 0x8E);
   setIdtEntry(35, (dword)irqIsr3, 0x08, 0x8E);
   setIdtEntry(36, (dword)irqIsr4, 0x08, 0x8E);
   setIdtEntry(37, (dword)irqIsr5, 0x08, 0x8E);
   setIdtEntry(38, (dword)irqIsr6, 0x08, 0x8E);
   setIdtEntry(39, (dword)irqIsr7, 0x08, 0x8E);
   setIdtEntry(40, (dword)irqIsr8, 0x08, 0x8E);
   setIdtEntry(41, (dword)irqIsr9, 0x08, 0x8E);
   setIdtEntry(42, (dword)irqIsr10, 0x08, 0x8E);
   setIdtEntry(43, (dword)irqIsr11, 0x08, 0x8E);
   setIdtEntry(44, (dword)irqIsr12, 0x08, 0x8E);
   setIdtEntry(45, (dword)irqIsr13, 0x08, 0x8E);
   setIdtEntry(46, (dword)irqIsr14, 0x08, 0x8E);
   setIdtEntry(47, (dword)irqIsr15, 0x08, 0x8E);
}

//http://wiki.osdev.org/PIC
//By default, in protected mode, IRQ interrupts conflict with CPU exception interrupts.  To fix 
//this, we'll remap the IRQs to after the CPU exceptions (0 to 0x1F) by reprogramming the PIC.  
//So, the master PIC's interrupts will start at 0x20, and the slave PIC's interrupts will start 
//at 0x28.
static void picRemap()
{
   byte pic1Mask = inb(pic1Data);
   byte pic2Mask = inb(pic2Data);

   outb(pic1Command, picInit);
   outb(pic2Command, picInit);

   outb(pic1Data, 0x20);
   outb(pic2Data, 0x28);

   outb(pic1Data, 4);
   outb(pic2Data, 2);

   outb(pic1Data, pic8086);
   outb(pic2Data, pic8086);

   outb(pic1Data, pic1Mask);
   outb(pic2Data, pic2Mask);
}

void initIdt()
{
   idtDesc.size = sizeof(idtEntries) - 1;
   idtDesc.baseAddress = (dword)&idtEntries;

   memset(&idtEntries, 0, sizeof(idtEntries));

   setCpuIsrEntries();
   setIrqIsrEntries();

   writeIdt((dword)&idtDesc);

   picRemap();
}


