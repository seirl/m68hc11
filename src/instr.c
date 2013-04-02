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
     * hashtable. This variadic macro "counts" the number of instructions by
     * adding +1 each time a new instruction is met */
    #define X(...) +1
    g_opcodes = hashtbl_init(0
        #include "opcodes.def"
    );
    #undef X

    /* This one actually adds the instructions in the hashtable */
    #define X(Mnemo, Rel, Inh, Imm, Dir, Ext, IndX, IndY)              \
        hashtbl_add(g_opcodes, #Mnemo,                                 \
        &((opcode) { #Mnemo, Rel, Inh, Imm, Dir, Ext, IndX, IndY }),   \
                sizeof(opcode));
    #include "opcodes.def"
    #undef X
}

/* Destroys the global list of opcodes */
void destroy_opcodes()
{
    hashtbl_destroy(g_opcodes);
}

/* Returns an opcode given its mnemonic */
opcode* get_opcode(const char* mnemo)
{
    char* upper = strtoupper(mnemo);
    opcode* res = hashtbl_find(g_opcodes, upper);
    free(upper);
    return res;
}
