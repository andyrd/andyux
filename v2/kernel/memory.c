#include "common.h"

void* memset(void* s, int c, int n)
{
    byte* p = s;

    while(n--) 
    {
       *p++ = (byte)c;
    }

    return s;
}

void* memcpy(void* d, const void* s, size_t c)
{
   byte* pD = (byte*)d;
   byte* pS = (byte*)s;
   byte* end = (byte*)(s + c);

   while(pS != end)
   {
      *(pD++) = *(pS++);
   }

   return d;
}
