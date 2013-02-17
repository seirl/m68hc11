#ifndef UTILS
#define UTILS
#include "core.h"

list_instr* add_list_instr(int opcode, int operand);
void free_list_instr(list_instr* list);
#endif
