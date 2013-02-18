#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "core.h"

list_instr* add_list_instr(int opcode, int operand)
{
    list_instr* element = malloc(sizeof(list_instr));
    element->opcode = opcode;
    element->operand = operand;
    element->next = NULL;
    return element;
}

void free_list_instr(list_instr* list)
{
    list_instr* to_free;
    while (list)
    {
        to_free = list;
        list = list->next;
        free(to_free);
    }
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
