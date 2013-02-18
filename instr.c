#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "core.h"
#include "hashtbl.h"
#include "utils.h"

hashtbl* g_opcodes;

void init_opcodes()
{
    #define X(A,B,C,D,E,F,G,H) +1
    g_opcodes = hashtbl_init(0
        #include "opcodes.def"
    );
    #undef X

    #define X(A,B,C,D,E,F,G,H)\
        hashtbl_add(g_opcodes, #A,\
        &((opcode) { #A,B,C,D,E,F,G,H }), sizeof(opcode));
    #include "opcodes.def"
    #undef X
}

opcode* get_opcode(char* mnemo)
{
    return (opcode*) hashtbl_find(g_opcodes, strtoupper(mnemo));
}
