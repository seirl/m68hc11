#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct opcode
{
    const char *mnemo;
    int rel;
    int inh;
    int imm;
    int dir;
    int ext;
    int indx;
    int indy;
};

typedef struct opcode opcode;

static opcode g_opcodes[] = {
#define X(A,B,C,D,E,F,G,H) { #A,B,C,D,E,F,G,H },
#include "opcodes.def"
#undef X
};

opcode* get_opcode(char* mnemo)
{
    unsigned int i;
    for (i = 0; i < (sizeof(g_opcodes) / sizeof(g_opcodes[0])); i++)
        if (!strcmp(mnemo, g_opcodes[i].mnemo))
            return &g_opcodes[i];
    return NULL;
}
