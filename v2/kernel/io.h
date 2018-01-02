#ifndef ANDYUX_IO_H
#define ANDYUX_IO_H

#include "common.h"

void outb(word port, byte val);
byte inb(word port);
void io_wait();

#endif
