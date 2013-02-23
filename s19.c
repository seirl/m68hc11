#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "core.h"
#include "utils.h"
#include "list.h"

#define S19_LENGTH 0x42


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
            *i++ = current->opcode / 0x100;
            *i++ = current->opcode & 0xFF;
        }
        else
            *i++ = current->opcode;

        if(current->operand > 0xFF)
        {
            *i++ = current->operand / 0x100;
            *i++ = current->operand % 0xFF;
        }
        else
            *i++ = current->operand;

        p = p->next;
    }
    return b;
}

void print_s19(list* list_instr)
{
    int s = size_list_instr(list_instr);
    int org = ((instr*)list_instr->start->data)->addr;
    unsigned char* b = to_bytes(list_instr, s);

    int checksum;

    int k;
    int i;
    int n = s / S19_LENGTH;
    int current_length = S19_LENGTH;
    int current_addr;

    for(k = 0; k < n + 1; k++)
    {
        if (k == n)
            current_length = s % S19_LENGTH;

        current_addr = org + S19_LENGTH * k;
        printf("S1%02X%04X", current_length + 3, current_addr);
        checksum = current_length + 3 + current_addr / 0x100 +
            (current_addr & 0xFF);
        for (i = 0; i < current_length; i++)
        {
            checksum += b[i + n * k];
            printf("%02X", b[i + n * k]);
        }
        printf("%02X\n", (0xFF - (checksum & 0xFF)));
    }
    printf("S9030000FC\n");
}
