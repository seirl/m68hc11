#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "core.h"
#include "utils.h"
#include "list.h"


/* Get the total size in bytes of the instruction list */
int size_list_instr(list* list_instr)
{
    instr* current;
    int bytes;
    current = list_instr->end->data;
    bytes = current->addr + current->size;
    current = list_instr->start->data;
    bytes -= current->addr;
    return bytes;
}

/* Transform a linked list of struct instr* to an array of bytes */
unsigned char* to_bytes(list* list_instr, const int s)
{
    unsigned char* b = calloc(s, sizeof(char));
    list_node* p = list_instr->start;
    instr* current;
    unsigned char* i = b;

    while (p)
    {
        current = p->data;
        if(current->opcode > 0xFF)
        {
            *i++ = current->opcode >> 8;
            *i++ = current->opcode & 0xFF;
        }
        else
            *i++ = current->opcode;

        if(current->operand > 0xFF)
        {
            *i++ = current->operand >> 8;
            *i++ = current->operand & 0xFF;
        }
        else
            *i++ = current->operand;

        p = p->next;
    }
    return b;
}

/* Generate and save the s19 output from an instruction list */
void fprint_s19(list* list_instr, FILE* f, const int default_length)
{
    int s = size_list_instr(list_instr);
    int org = ((instr*)list_instr->start->data)->addr;
    unsigned char* b = to_bytes(list_instr, s);

    int checksum;

    int k;
    int i;
    int n = s / default_length;
    int current_length = default_length;
    int current_addr;

    for(k = 0; k < n + 1; k++)
    {
        if (k == n)
            current_length = s % default_length;
        if(current_length)
        {
            current_addr = org + default_length * k;
            fprintf(f, "S1%02X%04X", current_length + 3, current_addr);
            checksum = current_length + 3 + current_addr / 0x100 +
                (current_addr & 0xFF);
            for (i = 0; i < current_length; i++)
            {
                checksum += b[i + n * k];
                fprintf(f, "%02X", b[i + n * k]);
            }
            fprintf(f, "%02X\n", (0xFF - (checksum & 0xFF)));
        }
    }
    free(b);
    fprintf(f, "S9030000FC\n");
    fclose(f);
}
