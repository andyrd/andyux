#include "screen.h"
#include "common.h"
#include "memory.h"
#include "io.h"

word* const videoRam = (word*)0xB8000;
const int screenWidth = 80;
const int screenHeight = 25;

const word crtcAddressRegister = 0x03D4;
const word crtcDataRegister = 0x03D5;

const byte cursorLocationLow = 0x0F;
const byte cursorLocationHigh = 0x0E;

const byte cursorStart = 0x0A;
const byte cursorEnd = 0x0B;

const byte colorGray = 0x07;
const byte space = 0x20;

static int cursorX = 0;
static int cursorY = 0;

static inline word getBlank()
{
   return (colorGray << 8) | space;
}

static void moveCursor()
{
   int cursorOffset = (cursorY * screenWidth) + cursorX;

   outb(crtcAddressRegister, cursorLocationLow);
   outb(crtcDataRegister, (byte)cursorOffset);

   outb(crtcAddressRegister, cursorLocationHigh);
   outb(crtcDataRegister, (byte)(cursorOffset >> 8));
}

static void scrollScreen()
{
   word blank = getBlank();

   for(int i = 0; i < (screenHeight - 1) * screenWidth; i++)
   {
      videoRam[i] = videoRam[i+screenWidth];
   }
   
   for(int j = (screenHeight - 1) * screenWidth; j < screenHeight * screenWidth; j++)
   {
      videoRam[j] = blank;
   }
}

int putchar(int c)
{
   const int backspace = 0x08;
   const int newline = '\n';
   const int carriageReturn = '\r';
   const int tab = 0x09;

   if(c == backspace && cursorX > 0)
   {
      cursorX--;
   }
   else if(c == carriageReturn)
   {
      cursorX = 0;
   }
   else if(c == newline)
   {
      cursorX = 0;
      cursorY++;
   }
   else if(c == tab)
   {
      //TODO: tab
   }
   else
   {
      int position = cursorY * screenWidth + cursorX;
      videoRam[position] = (colorGray << 8) | (byte)c;
      cursorX++;
   }

   if(cursorX >= screenWidth)
   {
      cursorX = 0;
      cursorY++;
   }
   
   if(cursorY >= screenHeight)
   {
      scrollScreen();
      cursorX = 0;
      cursorY = screenHeight - 1;
   }   
}

void clearScreen()
{   
   word blank = getBlank();

   for(int i = 0; i < screenWidth * screenHeight; i++)
   {
      videoRam[i] = blank;
   }

   cursorX = 0;
   cursorY = 0;
}

void showCursor()
{
   moveCursor();

   outb(crtcAddressRegister, cursorStart);
   byte cursorStartValue = inb(crtcDataRegister);

   outb(crtcAddressRegister, cursorStart);
   outb(crtcDataRegister, cursorStartValue & 0x1F); //clear bit five
}

void hideCursor()
{
   outb(crtcAddressRegister, cursorStart);
   byte cursorStartValue = inb(crtcDataRegister);

   outb(crtcAddressRegister, cursorStart);
   outb(crtcDataRegister, cursorStartValue | 0x20); //set bit five
}
