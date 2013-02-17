#include <stdlib.h>
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
