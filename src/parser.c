#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "core.h"
#include "instr.h"
#include "list.h"
#include "utils.h"
#include "hashtbl.h"

#define IDEN "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz._"
#define ERROR(msg) printf("%s:%d:%s\n", f, l, msg);

/* Metadata */
typedef struct {
    char name[1000];
    int org;
} meta;

/* Parse a numerical operand and returns its value and its addressing mode */
int parse_operand(const char* opr, int* operand, addressing* mode,
        const int l, const char* f)
{
    const char* opr_addr = opr;
    char* endptr;
    char base;
    int result;
    char index_reg;
    char error_msg[1000];

    *mode = DIR;
    /* Handle immediate mode */
    if (opr_addr[0] == '#')
    {
        *mode = IMM;
        opr_addr++;
    }
    base = opr_addr[0];
    switch (base)
    {
        case '%':   /* Binary */
            result = strtol(opr_addr + 1, &endptr, 2);
            break;
        case '@':   /* Octal */
            result = strtol(opr_addr + 1, &endptr, 8);
            break;
        case '$':   /* Hexadecimal */
            result = strtol(opr_addr + 1, &endptr, 16);
            break;
        case '\'':   /* ASCII */
            result = *(opr_addr + 1);
            /* We need an explicit cast here because of strtol() which takes a
             * char** instead of a const char** */
            endptr = (char*) opr_addr + 2;
            break;
        default:
            if ('1' <= base && base < '9') /* Decimal */
                result = strtol(opr_addr, &endptr, 10);
            else
            {
                sprintf(error_msg, "expected a number, optionally following "
                        "'%%', '@' or '$', found '%c'", base);
                ERROR(error_msg);
                return 1;
            }
    }
    *operand = result;
    opr_addr = endptr;

    /* Handle indexed addressing mode */
    if (opr_addr[0] == ',')
    {
        index_reg = opr_addr[1];
        if (*mode == IMM)
        {
            ERROR("register mode can't be both immediate and indexed");
            return 1;
        }
        else if (index_reg == '\0')
        {
            ERROR("expected a register name");
            return 1;
        }
        else if (opr_addr[2] != '\0')
        {
            sprintf(error_msg, "unexpected '%c'", opr_addr[3]);
            ERROR(error_msg);
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
                ERROR(error_msg);
                return 1;
            }
            opr_addr += 2;
        }
    }

    if (*operand > 0xFFFF)
    {
        sprintf(error_msg, "all operands must be less that 16 bytes "
                "(0x%X is too big)", *operand);
        ERROR(error_msg);
        return 1;
    }

    if (*mode == DIR && *operand > 0xFF)
    {
        *mode = EXT; /* Extended mode (16 bit operand) */
    }

    if ((*mode == INDX || *mode == INDY)  && *operand > 0xFF)
    {
        sprintf(error_msg, "offsets of indexed addressing modes must be less "
                "than 8 bytes (0x%X is too big)", *operand);
        ERROR(error_msg);
        return 1;
    }

    opr_addr = skip_blank(opr_addr);

    if (opr_addr[0] != '\0')
    {
        sprintf(error_msg, "unexpected '%c'", opr_addr[0]);
        ERROR(error_msg);
        return 1;
    }
    else
        return 0;
}

/* Return the opcode of an instruction given its addressing mode */
int opcode_from_mode(const opcode* instr, const addressing mode)
{
    switch(mode)
    {
        case REL:   return instr->rel;  break;
        case INH:   return instr->inh;  break;
        case IMM:   return instr->imm;  break;
        case DIR:   return instr->dir;  break;
        case EXT:   return instr->ext;  break;
        case INDX:  return instr->indx; break;
        case INDY:  return instr->indy; break;
        default:    return -1;          break;
    }
}

/* Tokenize a line in a label, an opcode and an operand */
int tokenize_line(const char* line, char* label, char* opcode, char* operand)
{
    const char* pline = line;
    char* ptarget = label;
    if(among(*pline, "*\n;"))
        return 0;

    /* Parse label */
    while(!is_blank(*pline))
        *ptarget++ = *pline++;
    if(ptarget - 1 >= line)
    {
        ptarget--;
        if (*ptarget != ':')
            ptarget++;
    }
    *ptarget = '\0';

    pline = skip_blank(pline);
    if(*pline == ';')
    {
        *opcode = '\0';
        *operand = '\0';
        return 1;
    }

    /* Parse opcode */
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

    /* Parse operand */
    ptarget = operand;
    while(!among(*pline, "\n;"))
        *ptarget++ = *pline++;
    *ptarget = '\0';

    return 1;
}

/* Evaluate the current line and add the instruction found in the list */
int eval_line(const char* line, meta* mdata, const int l, const char* f,
        list* current, hashtbl* names)
{
    char error_msg[1000];
    char label[1000];
    char instr_name[1000];
    char opr[1000];

    int operand;
    opcode* opc = NULL;
    int has_operand;
    addressing mode;
    instr* i;
    static int current_addr = 0;

    int* idata;

    if(!tokenize_line(line, label, instr_name, opr))
        return 0;

    /* Add labels to the environment hashtable */
    if (*label)
    {
        idata = malloc(sizeof(int));
        if (!strcmp(instr_name, "EQU"))
        {
            parse_operand(opr, &operand, &mode, l, f);
            if (mode != DIR && mode != EXT)
            {
                ERROR("addressing modes cannot be used with assembly "
                        "directives");
                return 1;
            }
            *idata = operand;
            hashtbl_add(names, label, idata, sizeof(int));
            return 0;
        }
        else
        {
            *idata = current_addr;
            hashtbl_add(names, label, idata, sizeof(int));
        }
    }

    if (!*instr_name)
        return 0;

    /* Handle assembly directives */
    if (!strcmp(instr_name, "NAME"))
    {
        strcpy(mdata->name, opr);
        return 0;
    }
    else if (!strcmp(instr_name, "END"))
        return 2;
    else if (!strcmp(instr_name, "ORG"))
    {
        if (parse_operand(opr, &operand, &mode, l, f))
            return 1;
        if (mode != DIR && mode != EXT)
        {
            ERROR("addressing modes cannot be used with assembly directives");
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
            ERROR(error_msg);
            return 1;
        }

        if (!mdata->org)
        {
            ERROR("found an instruction before the 'org' directive");
            return 1;
        }

        has_operand = (opcode_from_mode(opc, INH) == -1);
        if (has_operand)
        {
            if (!*opr)
            {
                ERROR("missing operand");
                return 1;
            }

            /* Handle operand names */
            if (among(*opr, IDEN))
            {
                if (opc->rel == -1)
                {
                    idata = (int*) hashtbl_find(names, opr);
                    if(!idata)
                    {
                        sprintf(error_msg, "name %s undeclared", opr);
                        ERROR(error_msg);
                        return 1;
                    }
                    operand = *idata;
                    mode = (operand < 0xFF) ? DIR : EXT;
                    i = create_instr(opcode_from_mode(opc, mode), operand);
                    i->ref = NULL;
                    i->addr = current_addr;
                }
                else
                {
                    i = create_instr(opcode_from_mode(opc, REL), 0);
                    i->ref = calloc(strlen(opr) + 1, sizeof(char));
                    strcpy(i->ref, opr);
                    i->addr = current_addr;
                }
            }
            else
            {
                if (parse_operand(opr, &operand, &mode, l, f))
                    return 1;
                if (!has_operand)
                    mode = INH;
                if (opcode_from_mode(opc, mode) == -1)
                {
                    if (opc->rel != -1)
                        mode = REL;
                    else
                    {
                        sprintf(error_msg,
                                "cannot find any opcode associated with '%s'",
                                instr_name);
                        ERROR(error_msg);
                        return 1;
                    }
                }
                i = create_instr(opcode_from_mode(opc, mode), operand);
                i->addr = current_addr;
                i->ref = NULL;
            }
        }
        current_addr += i->size;
        i->line = l;

        /* Actually add the instruction to the list */
        list_append(current, i, sizeof(instr));

        free(i);
        return 0;
    }
}

/* Second pass: resolve branching addresses */
int second_pass(list* list_instr, hashtbl* names, const char* f)
{
    list_node* p = list_instr->start;
    instr* current;
    int* ref;
    char error_msg[1000];
    int errn = 0;
    int l;

    while (p)
    {
        current = p->data;
        if (current->ref)
        {
            ref = (int*) hashtbl_find(names, current->ref);
            l = current->line;
            if (!ref)
            {
                sprintf(error_msg, "name %s undeclared", current->ref);
                ERROR(error_msg);
                errn++;
            }
            else
            {
                if (abs(*ref - current->addr) > 127)
                {
                    sprintf(error_msg, "%s label is too far (max offset is "
                            "127, current is %d)", current->ref,
                            abs(*ref - current->addr));
                    ERROR(error_msg);
                    errn++;
                }
                else
                {
                    current->operand = *ref - current->addr;
                }
                current->ref = NULL;
                free(current->ref);
            }
        }
        p = p->next;
    }
    return (errn > 0);
}

/* Main parse function, return an instruction list */
list* parse(FILE* stream, char* name, const char* f)
{
    char line[1000];
    char* read;
    int n = 0;
    int errn = 0;
    int l = 0;
    meta mdata = { "", 0 };
    list* list_instr;
    hashtbl* names = hashtbl_init(127);

    list_instr = list_init();

    /* Iterate over the lines of the file */
    while ((read = fgets(line, 999, stream)) != NULL)
    {
        l++;

        /* Handle too long lines */
        if(read[strlen(read) - 1] != '\n')
        {
            ERROR("line too long");
            errn++;
            break;
        }
        /* Actual work on the current line */
        n = eval_line(line, &mdata, l, f, list_instr, names);
        if (n == 2)
            break;
        else if (n == 1)
            errn++;
    }
    l = 0;
    if (!mdata.name)
    {
        ERROR("'name' directive not found");
        errn++;
    }
    strcpy(name, mdata.name);

    /* Second pass : resolve labels */
    if(second_pass(list_instr, names, f))
        errn++;

    hashtbl_destroy(names);

    if (!list_instr->count && !errn)
    {
        ERROR("no instructions found");
        errn++;
    }
    return (errn) ? NULL : list_instr;
}
