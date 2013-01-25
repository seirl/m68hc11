#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

int main(void)
{
    FILE* stream = fopen("test.asm", "r");
    if (!stream)
    {
        return EXIT_FAILURE;
    }
    parse(stream);
    fclose(stream);
    return EXIT_SUCCESS;
}
