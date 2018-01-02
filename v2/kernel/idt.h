#ifndef ANDYUX_IDT_H
#define ANDYUX_IDT_H

#include "common.h"

extern const word pic1Command;
extern const word pic1Data;
extern const word pic2Command;
extern const word pic2Data;
extern const byte picInit;
extern const byte pic8086;
extern const byte picEoi;

struct idtDescriptorStruct 
{
   word size;
   dword baseAddress;
} __attribute__((packed));

struct idtEntryStruct
{
   word baseLow;
   word segmentSelector;
   byte alwaysZero;
   byte flags;
   word baseHigh;
} __attribute__((packed));

typedef struct idtDescriptorStruct idtDescriptor;
typedef struct idtEntryStruct idtEntry;

void initIdt();

#endif
