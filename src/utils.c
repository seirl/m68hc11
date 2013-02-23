#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "core.h"

int number_of_bytes(const instr* i)
{
    if (i->opcode > 0xFFFF || i->operand > 0xFFFF ||
            i->opcode < 0 || i->operand < 0)
        return -1;
    return 2 + (i->operand > 0xFF) + (i->opcode > 0xFF);
}

instr* create_instr(const int opcode, const int operand)
{
    instr* i;
    i = malloc(sizeof(instr));
    i->opcode = opcode;
    i->operand = operand;
    i->size = number_of_bytes(i);
    return i;
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
    char* dst = calloc((strlen(src) + 1), sizeof(char));
    char* r = dst;
    while (*src)
    {
        *dst = (unsigned char) toupper((int)*src++);
        dst++;
    }
    return r;
}

int among(const char c, const char* str)
{
    while(*str)
        if(*str++ == c)
            return 1;
    return 0;
}

int is_blank(const char c)
{
    return among(c, " \t\n");
}

const char* skip_blank(const char* c)
{
    while(among(*c, " \t"))
         c++; /* lol. */
    return c;
}