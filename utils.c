#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "core.h"

int number_of_bytes(instr* i)
{
    if (i->opcode > 0xFFFF || i->operand > 0xFFFF ||
            i->opcode < 0 || i->operand < 0)
        return -1;
    return 2 + (i->operand > 0xFF) + (i->opcode > 0xFF);
}

int stricmp (const char *p1, const char *p2)
{
  register unsigned char *s1 = (unsigned char *) p1;
  register unsigned char *s2 = (unsigned char *) p2;
  unsigned char c1, c2;
  do
  {
      c1 = (unsigned char) toupper((int)*s1++);
      c2 = (unsigned char) toupper((int)*s2++);
      if (c1 == '\0')
            return c1 - c2;
  }
  while (c1 == c2);
  return c1 - c2;
}

char* strtoupper(const char* src)
{
    char* dst = malloc(sizeof(char) * strlen(src));
    char* r = dst;
    while (*src)
    {
        *dst = (unsigned char) toupper((int)*src++);
        dst++;
    }
    return r;
}
