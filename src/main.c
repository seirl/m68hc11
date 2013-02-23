#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "parser.h"
#include "core.h"
#include "instr.h"
#include "s19.h"

int main(int argc, char* argv[])
{
    FILE* stream;
    list* l;
    char name[1000];
    init_opcodes();
    int i;
    int s19_default_length = 0x42;
    if (argc <= 1 ||
        (argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))))
    {
        printf("Usage: %s [-h | -s N] file1.asm [file2.asm […]]\n", argv[0]);
        printf("\t-h\tdisplay help\n");
        printf("\t-s N\twrite N instructions per s19 record\n");
    }
    else
    {
        for (i = 1; i < argc; i++)
        {
            if (!strcmp(argv[i], "-s"))
            {
                s19_default_length = strtol(argv[++i], NULL, 10);
                if(s19_default_length < 1)
                {
                    printf("'%s' is not a valid length\n", argv[i]);
                    return EXIT_FAILURE;
                }
            }
            else
            {
                stream = fopen(argv[i], "r");
                if (!stream)
                {
                    printf("'%s' not found\n", argv[i]);
                    return EXIT_FAILURE;
                }
                l = parse(stream, name, argv[i]);
                if (!l)
                    return EXIT_FAILURE;
                strcat(name, ".s19");
                fprint_s19(l, name, s19_default_length);
                fclose(stream);
            }
        }
    }
    return EXIT_SUCCESS;
}
