#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "base.h"
#include "instr.h"

void error(char* msg, int line)
{
    printf("Error line %d: %s\n", line, msg);
}

typedef struct {
    char name[1000];
} meta;

int parse_operand(char* opr, int* operand, addressing* mode, int l)
{
    char* opr_addr = opr;
    char* endptr;
    char base;
    int result;
    char index_reg;
    char error_msg[1000];

    *mode = DIR;
    if (opr_addr[0] == '#')
    {
        *mode = IMM;
        opr_addr++;
    }
    base = opr_addr[0];
    switch (base)
    {
        case '%':
            result = strtol(opr_addr + 1, &endptr, 2);
            break;
        case '$':
            result = strtol(opr_addr + 1, &endptr, 16);
            break;
        default:
            sprintf(error_msg, "expected '%%' or '$', found '%c'", base);
            error(error_msg, l);
            return 1;
    }
    *operand = result;
    opr_addr = endptr;
    if (opr_addr[0] == ',')
    {
        index_reg = opr_addr[1];
        if (*mode == IMM)
        {
            error("register mode can't be both immediate and indexed", l);
        }
        else if (index_reg == '\0')
        {
            error("expected a register name", l);
        }
        else if (opr_addr[3] != '\0')
        {
            sprintf(error_msg, "unexpected '%c'", opr_addr[3]);
            error(error_msg, l);
        }
        else
        {
            if (index_reg == 'X')
                *mode = INDX;
            else if (index_reg == 'Y')
                *mode = INDY;
            else
            {
                sprintf(error_msg, "expected 'X' or 'Y', found '%c'",
                        index_reg);
                error(error_msg, l);
                return 1;
            }
            opr_addr += 2;
        }
    }
    else
    {
        if (*operand <= 0xFF)
            *mode = DIR;
        else
            *mode = EXT;
    }

    if (opr_addr[0] != '\0')
    {
        sprintf(error_msg, "unexpected '%c'", opr_addr[0]);
        error(error_msg, l);
        return 1;
    }
    else
        return 0;
}

int opcode_from_mode(opcode* instr, addressing mode)
{
    switch(mode)
    {
        case REL:
            return instr->rel;
            break;
        case INH:
            return instr->inh;
            break;
        case IMM:
            return instr->imm;
            break;
        case DIR:
            return instr->dir;
            break;
        case EXT:
            return instr->ext;
            break;
        case INDX:
            return instr->indx;
            break;
        case INDY:
            return instr->indy;
            break;
        default:
            return -1;
            break;
    }
}

int parse_expr(char* line, meta* mdata, int line_number)
{
    char instr[5];
    char opr[32];
    char error_msg[1000];
    int operand;
    opcode* opc;
    int has_operand;
    int endmatch;
    addressing mode;

    if (sscanf(line, "%s%n", instr, &endmatch) < 1)
    {
        error("unexpected end of file", line_number);
        return 1;
    }

    if (!strcmp(instr, "name"))
    {
        strcpy(mdata->name, instr);
        return 0;
    }
    else if (!strcmp(instr, "org"))
        return 0;
    else if (!strcmp(instr, "end"))
        return 2;

    if ((opc = get_opcode(instr)) == NULL)
    {
        sprintf(error_msg, "instruction '%.5s' unknown ", instr);
        error(error_msg, line_number);
        return 1;
    }

    has_operand = (opc->inh == -1);
    if (has_operand)
    {
        if (sscanf(line + endmatch, "%s", opr) < 1)
        {
            error("missing operand", line_number);
            return 1;
        }
        
        if (parse_operand(opr, &operand, &mode, line_number))
            return 1;
    }
    else
        mode = INH;

    if (opcode_from_mode(opc, mode) == -1)
        mode = REL;
    if (opcode_from_mode(opc, mode) == -1)
    {
        sprintf(error_msg, "cannot find any opcode associated with '%s'",
                instr);
        error(error_msg, line_number);
        return 1;
    }
    printf("%X\n", opcode_from_mode(opc, mode));
    return 0;
}

void parse(FILE* stream)
{
    char line[50];
    char* read;
    int i = 0;
    meta mdata;
    while ((read = fgets(line, 49, stream)) != NULL)
    {
        i++;
        if (parse_expr(line, &mdata, i) > 0)
            break;
    }
}
