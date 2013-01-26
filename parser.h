void parse(FILE* stream);

typedef enum
{
    INH, /* Inherent (for special instructions that doesn't need mem-access) */
    IMM, /* Immediate (actual data follows the opcode, 8b or 16b operands) */
    DIR, /* Direct (8b data following the opcode is the @ of the operand) */
    EXT, /* Extended (similar than direct but for 16b adresses) */
    INDX,/* Indexed (address of operand calculated by offset + X) */
    INDY,/* Indexed (address of operand calculated by offset + Y) */
} addressing;
