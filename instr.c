#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "core.h"
#include "hashtbl.h"
#include "utils.h"

hashtbl* g_opcodes;

void init_opcodes()
{
   opcode o;
   g_opcodes = hashtbl_init(0

#define X(...) +1
#include "opcodes.def"
#undef X

           );

#define X(A,B,C,D,E,F,G,H)                          \
        o = (opcode) { #A,B,C,D,E,F,G,H };          \
        hashtbl_add(g_opcodes, #A, &o, sizeof(o));  \

#include "opcodes.def"
#undef X

}

opcode* get_opcode(char* mnemo)
{
    return (opcode*) hashtbl_find(g_opcodes, strtoupper(mnemo));
}
