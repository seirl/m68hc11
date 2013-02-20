#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "core.h"
#include "hashtbl.h"
#include "utils.h"

hashtbl* g_opcodes;

/* This function loads the instructions defined in "opcodes.def" in the
 * hashtable g_opcodes */
void init_opcodes()
{
    /* I want to know the number of instructions to create an efficient
     * hashtable. This macro "counts" the number of instructions by adding +1
     * each time a new instruction is met */
    #define X(A,B,C,D,E,F,G,H) +1
    g_opcodes = hashtbl_init(0
        #include "opcodes.def"
    );
    #undef X

    /* This one actually adds the instructions in the hashtable */
    #define X(A,B,C,D,E,F,G,H)\
        hashtbl_add(g_opcodes, #A,\
        &((opcode) { #A,B,C,D,E,F,G,H }), sizeof(opcode));
    #include "opcodes.def"
    #undef X
}

opcode* get_opcode(const char* mnemo)
{
    return (opcode*) hashtbl_find(g_opcodes, strtoupper(mnemo));
}
