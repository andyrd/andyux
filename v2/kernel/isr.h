#ifndef ANDYUX_ISR_H
#define ANDYUX_ISR_H

#include "common.h"

extern const byte irq0;
extern const byte irq1;
extern const byte irq2;
extern const byte irq3;
extern const byte irq4;
extern const byte irq5;
extern const byte irq6;
extern const byte irq7;
extern const byte irq8;
extern const byte irq9;
extern const byte irq10;
extern const byte irq11;
extern const byte irq12;
extern const byte irq13;
extern const byte irq14;
extern const byte irq15;

struct registersStruct
{
   dword ds;
   dword edi, esi, ebp, esp, ebx, edx, ecx, eax;
   dword intNumber, errorCode;
   dword eip, cs, eflags, useresp, ss;
};

typedef struct registersStruct registers;
typedef void (*isr)(registers);

void cpuIsrRouter(registers regs);
void irqIsrRouter(registers regs);
void clearInterruptHandlers();
void registerInterruptHandler(byte interrupt, isr handler);
void irqSetMask(byte interrupt);
void irqClearMask(byte interrupt);
void disableInterrupts();
void enableInterrupts();

#endif
