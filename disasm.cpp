#include <stdio.h>
#include "spu.hpp"


int main ()
{
    FILE* infile = fopen ("asm.txt", "r");
    FILE* outfile  = fopen ("disasm.txt", "w");

    int x = 0;

    while (x != cmd_HLT)
    {
        fscanf (infile, "%d", &x);
        elem_t elem = 0;
        switch (x)
        {

        case cmd_push:
            fscanf (infile, "%d", &elem);
            fprintf (outfile, "push %d\n", elem);
            break;

        case cmd_add:
            fputs ("add\n", outfile);
            break;

        case cmd_sub:
            fputs ("sub\n", outfile);
            break;

        case cmd_mul:
            fputs ("mul\n", outfile);
            break;

        case cmd_div:
            fputs ("divi\n", outfile);
            break;

        case cmd_in:
            fputs ("in\n", outfile);
            break;

        case cmd_out:
            fputs ("out\n", outfile);
            break;

        case cmd_rpush:
            fscanf (infile, "%d", &elem);
            fputs ("push ", outfile);
            switch (elem)
            {
                case ax:
                    fputs ("ax\n", outfile);
                    break;
                case bx:
                    fputs ("bx\n", outfile);
                    break;
                case cx:
                    fputs ("cx\n", outfile);
                    break;
                case dx:
                    fputs ("dx\n", outfile);
                    break;
            }
            break;

        case cmd_pop:
            fscanf (infile, "%d", &elem);
            fputs ("pop ", outfile);
            switch (elem)
            {
                case ax:
                    fputs ("ax\n", outfile);
                    break;
                case bx:
                    fputs ("bx\n", outfile);
                    break;
                case cx:
                    fputs ("cx\n", outfile);
                    break;
                case dx:
                    fputs ("dx\n", outfile);
                    break;
            }
            break;

        }
    }
    fputs ("HLT\n", outfile);
    fclose (infile);
    fclose (outfile);
}
