#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void error(char* msg, int line)
{
    printf("Error l%d: %s\n", line, msg);
}

typedef enum
{
    INH, /* Inherent (for special instructions that doesn't need mem-access) */
    IMM, /* Immediate (actual data follows the opcode, 8b or 16b operands) */
    DIR, /* Direct (8b data following the opcode is the @ of the operand) */
    EXT, /* Extended (similar than direct but for 16b adresses) */
    INDX,/* Indexed (address of operand calculated by offset + X) */
    INDY,/* Indexed (address of operand calculated by offset + Y) */
} addressing;

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
        *opr_addr += 1;
    }
    base = opr_addr[0];
    switch (base)
    {
        case '%':
            result = strtol(opr_addr, &endptr, 2);
            break;
        case '$':
            result = strtol(opr_addr, &endptr, 16);
            break;
        default:
            sprintf(error_msg, "expected '%%' or '$', found %c", base);
            error(error_msg, l);
            return 1;
    }
    *operand = result;
    if (opr_addr == endptr)
    {
        if (*operand > 0xFF)
            *mode = DIR;
        else
            *mode = EXT;
        return 0;
    }
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
            sprintf(error_msg, "unexpected %c", opr_addr[3]);
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
                sprintf(error_msg, "expected 'X' or 'Y', found %c",
                        index_reg);
                error(error_msg, l);
                return 1;
            }
            return 0;
        }
        return 1;
    }
    else
    {
        sprintf(error_msg, "unexpected %c", opr_addr[0]);
        error(error_msg, l);
        return 1;
    }
}

int parse_expr(char* line, int line_number)
{
    char instr[5];
    char opr[32];
    int operand;
    if (sscanf(line, "%s %s\n", instr, opr) < 2)
    {
        if (sscanf(line, "%s\n", instr) < 1)
        {
            error("cannot parse line", line_number);
            return 1;
        }
        else
        {
            if (!strcmp("end", instr))
            {
                return 2;
            }
            else
            {
                return 0;
            }
        }
    }
    else
    {
        if (!strcmp("name", instr))
        {
            return 0;
        }
        addressing mode;
        return parse_operand(opr, &operand, &mode, line_number);
    }
}

void parse(FILE* stream)
{
    char line[50];
    char* read;
    int i = 0;
    while ((read = fgets(line, 49, stream)) != NULL)
    {
        i++;
        if (parse_expr(line, i) > 0)
            break;
    }
}
