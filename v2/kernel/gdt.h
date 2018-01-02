#ifndef ANDYUX_GDT_H
#define ANDYUX_GDT_H

#include "common.h"

struct gdtDescriptorStruct
{
   word size;
   dword baseAddress;
} __attribute__((packed));

struct gdtEntryStruct
{
   word limitLow;
   word baseLow;
   byte baseMiddle;
   byte access;
   byte limitHighAndFlags;
   byte baseHigh;
} __attribute__((packed));

typedef struct gdtDescriptorStruct gdtDescriptor;
typedef struct gdtEntryStruct gdtEntry;

void initGdt();

#endif
