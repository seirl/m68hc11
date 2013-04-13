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
    int display = 0;
    FILE* f;
    if (argc <= 1 ||
        (argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))))
    {
        /* Display help */
        printf("Usage: %s [-h | -d] file1.asm [file2.asm […]]\n", argv[0]);
        printf("\t-h\tdisplay help\n");
        printf("\t-d\tdisplay the output to stdout\n");
    }
    else
    {
        /* Iterate over the args & files to parse */
        for (i = 1; i < argc; i++)
        {
            /* Handle options */
            if (!strcmp(argv[i], "-d"))
            {
                display = 1;
            }
            else
            {
                stream = fopen(argv[i], "r");
                if (!stream)
                {
                    printf("'%s' not found\n", argv[i]);
                    return EXIT_FAILURE;
                }

                /* Parse the file and get the name of the result */
                l = parse(stream, name, argv[i]);
                if (!l)
                {
                    printf("%s: *** Error 1\n", argv[i]);
                    return EXIT_FAILURE;
                }
                strcat(name, ".s19");

                if (display)
                    f = stdout;
                else
                    f = fopen(name, "w");
                /* Generate and save the s19 output */
                fprint_s19(l, f, 32);
                list_destroy(l);
                fclose(stream);
            }
        }
    }
    destroy_opcodes();
    return EXIT_SUCCESS;
}
