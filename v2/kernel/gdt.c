#include "gdt.h"

gdtEntry gdtEntries[5];
gdtDescriptor gdtDesc;

extern void writeGdt(dword descriptorAddress);

static void setGdtEntry(int num, dword base, dword limit, byte access, byte flags)
{
   gdtEntries[num].baseLow = base & 0xFFFF;
   gdtEntries[num].baseMiddle = (base >> 16) & 0xFF;
   gdtEntries[num].baseHigh = (base >> 24) & 0xFF;

   gdtEntries[num].limitLow = limit & 0xFFFF;
   gdtEntries[num].limitHighAndFlags = ((limit >> 16) & 0x0F) | (flags & 0xF0);
   
   gdtEntries[num].access = access;
}

void initGdt()
{
   gdtDesc.size = sizeof(gdtEntries) - 1;
   gdtDesc.baseAddress = (dword)&gdtEntries;

   //for the break down of the access and flag values, http://wiki.osdev.org/Global_Descriptor_Table

   setGdtEntry(0, 0, 0, 0, 0);                //null segment
   setGdtEntry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); //kernel code segment
   setGdtEntry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); //kernel data segment
   setGdtEntry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); //user mode code segment
   setGdtEntry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); //user mode data segment

   writeGdt((dword)&gdtDesc);
}

