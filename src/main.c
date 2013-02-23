#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "parser.h"
#include "core.h"
#include "instr.h"
#include "s19.h"

int main(int argc, char* argv[])
{
    FILE* stream;
    list* l;
    init_opcodes();
    stream = fopen("test.asm", "r");
    if (!stream)
        return EXIT_FAILURE;
    l = parse(stream);
    print_s19(l);
    fclose(stream);
    return EXIT_SUCCESS;
}
