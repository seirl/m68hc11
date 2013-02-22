#ifndef BASE
#define BASE

typedef enum
{
    REL, /* Relative (used for branch instructions) */
    INH, /* Inherent (for special instructions that doesn't need mem-access) */
    IMM, /* Immediate (actual data follows the opcode, 8b or 16b operands) */
    DIR, /* Direct (8b data following the opcode is the @ of the operand) */
    EXT, /* Extended (similar than direct but for 16b adresses) */
    INDX,/* Indexed (address of operand calculated by offset + X) */
    INDY, /* Indexed (address of operand calculated by offset + Y) */
    NO, /* Nothing */
} addressing;

typedef struct
{
    const char *mnemo;
    int rel;
    int inh;
    int imm;
    int dir;
    int ext;
    int indx;
    int indy;
} opcode;


typedef struct
{
    int opcode;
    int operand;
    char* ref;
    int size;
    int addr;
} instr;

typedef struct
{
    int opcode;
    char* ref;
    int size;
    int addr;
} refto;

typedef enum
{
    ST_LABEL,
    ST_INSTRUCTION,
    ST_REFTO
} statement_type;

typedef struct
{
    statement_type t;
    union
    {
        instr* instruction;
        char* label;
        refto* reference_to;
    } u;
} statement;

#endif
