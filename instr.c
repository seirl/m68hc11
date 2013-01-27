#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "base.h"

static opcode g_opcodes[] = {
#define X(A,B,C,D,E,F,G,H) { #A,B,C,D,E,F,G,H },
#include "opcodes.def"
#undef X
};

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

opcode* get_opcode(char* mnemo)
{
    unsigned int i;
    for (i = 0; i < (sizeof(g_opcodes) / sizeof(g_opcodes[0])); i++)
        if (!stricmp(mnemo, g_opcodes[i].mnemo))
            return &g_opcodes[i];
    return NULL;
}
