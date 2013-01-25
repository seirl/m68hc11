#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const struct
{
    const char *mnemo;
    int rel;
    int inh;
    int imm;
    int dir;
    int ext;
    int indx;
    int indy;
} g_opcodes[] = {
#define X(A,B,C,D,E,F,G,H) { #A,B,C,D,E,F,G,H },
#include "opcodes.def"
#undef X
};
