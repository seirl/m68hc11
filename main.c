#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "parser.h"
#include "core.h"
#include "instr.h"

int main(void)
{
    FILE* stream;
    list* l;
    list_node* p;
    instr* current;
    init_opcodes();
    stream = fopen("test.asm", "r");
    if (!stream)
        return EXIT_FAILURE;
    l = parse(stream);
    if (l)
    {
        p = l->start;
        while (p)
        {
            current = p->data;
            printf("0x%X: 0x%X %d\n",
                    current->addr, current->opcode, current->operand);
            p = p->next;
        }
    }
    fclose(stream);
    return EXIT_SUCCESS;
}
