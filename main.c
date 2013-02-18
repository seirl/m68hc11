#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "core.h"
#include "instr.h"

int main(void)
{
    FILE* stream;
    init_opcodes();
    stream = fopen("test.asm", "r");
    if (!stream)
    {
        return EXIT_FAILURE;
    }
    parse(stream);
    fclose(stream);
    return EXIT_SUCCESS;
}
