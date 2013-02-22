#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "core.h"
#include "instr.h"
#include "list.h"
#include "utils.h"

void error(char* msg, int line)
{
    printf("Error line %d: %s\n", line, msg);
}

typedef struct {
    char name[1000];
    int org;
} meta;

int parse_operand(const char* opr, int* operand, addressing* mode, const int l)
{
    const char* opr_addr = opr;
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
        case '@':
            result = strtol(opr_addr + 1, &endptr, 8);
            break;
        case '$':
            result = strtol(opr_addr + 1, &endptr, 16);
            break;
        default:
            if ('1' <= base && base < '9')
                result = strtol(opr_addr, &endptr, 10);
            else
            {
                sprintf(error_msg, "expected a number, optionally following "
                        "'%%', '@' or '$', found '%c'", base);
                error(error_msg, l);
                return 1;
            }
    }
    *operand = result;
    opr_addr = endptr;
    if (opr_addr[0] == ',')
    {
        index_reg = opr_addr[1];
        if (*mode == IMM)
        {
            error("register mode can't be both immediate and indexed", l);
            return 1;
        }
        else if (index_reg == '\0')
        {
            error("expected a register name", l);
            return 1;
        }
        else if (opr_addr[3] != '\0')
        {
            sprintf(error_msg, "unexpected '%c'", opr_addr[3]);
            error(error_msg, l);
            return 1;
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

    if (*operand > 0xFFFF)
    {
        sprintf(error_msg, "all operands must be less that 16 bytes "
                "(0x%X is too big)", *operand);
        error(error_msg, l);
        return 1;
    }

    if (*mode == DIR && *operand > 0xFF)
    {
        *mode = EXT;
    }

    if ((*mode == INDX || *mode == INDY)  && *operand > 0xFF)
    {
        sprintf(error_msg, "offsets of indexed addressing modes must be less "
                "than 8 bytes (0x%X is too big)", *operand);
        error(error_msg, l);
        return 1;
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

int tokenize_line(char* line, char* label, char* opcode, char* operand)
{
    char* pline = line;
    char* ptarget = label;
    if(among(*pline, "*\n;"))
        return 0;

    /* Parsing label */
    while(!is_blank(*pline))
        *ptarget++ = *pline++;
    ptarget--;
    if (*ptarget != ':')
        ptarget++;
    *ptarget = '\0';

    pline = skip_blank(pline);
    if(*pline == ';')
    {
        *opcode = '\0';
        *operand = '\0';
        return 1;
    }

    /* Parsing opcode */
    ptarget = opcode;
    while(!is_blank(*pline))
        *ptarget++ = (unsigned char) toupper((int)*pline++);
    *ptarget = '\0';

    pline = skip_blank(pline);
    if(*pline == ';')
    {
        *operand = '\0';
        return 1;
    }

    /* Parsing operand */
    ptarget = operand;
    while(!among(*pline, "\n;"))
        *ptarget++ = *pline++;
    *ptarget = '\0';

    return 1;
}

int parse_expr(char* line, meta* mdata, const int line_number, list* current)
{
    char error_msg[1000];
    char label[300];
    char instr_name[300];
    char opr[300];

    int operand;
    opcode* opc;
    int has_operand;
    addressing mode;
    statement* st;
    static int current_addr = 0;

    if(!tokenize_line(line, label, instr_name, opr))
        return 0;

    if (*label)
        list_append(current, create_label(label), sizeof(statement));

    if (!*instr_name)
        return 0;

    if (!strcmp(instr_name, "NAME"))
    {
        strcpy(mdata->name, instr_name);
        return 0;
    }
    else if (!strcmp(instr_name, "END"))
        return 2;
    else if (!strcmp(instr_name, "ORG"))
    {
        if (parse_operand(opr, &operand, &mode, line_number))
            return 1;
        if (mode != DIR && mode != EXT)
        {
            error("adressing modes cannot be used with assembly directives",
                    line_number);
            return 1;
        }
        mdata->org = operand;
        current_addr = operand;
        return 0;
    }
    else
    {
        if ((opc = get_opcode(instr_name)) == NULL)
        {
            sprintf(error_msg, "instruction '%.5s' unknown ", instr_name);
            error(error_msg, line_number);
            return 1;
        }

        if (!mdata->org)
        {
            error("found an instruction before the 'org' directive",
                    line_number);
            return 1;
        }

        has_operand = (opcode_from_mode(opc, INH) == -1);

        if (has_operand)
        {
            if (!*opr)
            {
                error("missing operand", line_number);
                return 1;
            }

            if (among(*opr,
                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                        "abcdefghijklmnopqrstuvwxyz"
                        "0123456789._"))
            {}
            if (parse_operand(opr, &operand, &mode, line_number))
                return 1;
        }

        if (!has_operand)
            mode = INH;
        if (opcode_from_mode(opc, mode) == -1 && opc->rel != -1)
            mode = REL;
        else
        {
            sprintf(error_msg, "cannot find any opcode associated with '%s'",
                    instr_name);
            error(error_msg, line_number);
            return 1;
        }


        st = create_instr(opcode_from_mode(opc, mode), operand);
        st->u.instruction->addr = current_addr;
        current_addr += st->u.instruction->size;
        list_append(current, st, sizeof(statement));
        return 0;
    }
}

list* parse(FILE* stream)
{
    char line[1000];
    char* read;
    int i = 0;
    meta mdata;
    list* list_instr;
    list_node* p;
    instr* current;
    refto* current_ref;
    statement* st;

    list_instr = list_init();
    while ((read = fgets(line, 999, stream)) != NULL)
    {
        i++;
        if (parse_expr(line, &mdata, i, list_instr) > 0)
            break;
    }

    p = list_instr->start;
    while (p)
    {
        st = p->data;
        if (st->t == ST_INSTRUCTION)
        {
            current = st->u.instruction;
            printf("0x%X: 0x%X 0x%X\n",
                    current->addr, current->opcode, current->operand);
        }
        else if (st->t == ST_REFTO)
        {
            current_ref = st->u.reference_to;
            printf("0x%X: 0x%X <%s\n>",
                    current_ref->addr, current_ref->opcode, current_ref->ref);
        }
        else if (st->t == ST_LABEL)
        {
            printf(":%s\n", st->u.label);
        }
        p = p->next;
    }
    return list_instr;
}
