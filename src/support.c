#include <nusys.h>

void __cxa_pure_virtual(void) {
  for (;;) {}
}

void * memset(void *b, int c, size_t len)
{
  int i = 0;
  unsigned char * p = b;
  while(len > 0)
    {
      *p = c;
      p++;
      len--;
    }
  return(b);
}
