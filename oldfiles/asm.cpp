#include <stdio.h>
#include <string.h>
#include "spu.hpp"

int main ()
{
    FILE* infile = fopen ("proga.txt", "r");
    FILE* outfile = fopen ("asm.txt", "w");

    for (size_t line = 0;; line++)
    {
        char s[12] = "";
        fscanf (infile, "%s", s);

        if (strcmp (s, "HLT") == 0)
        {
            fprintf (outfile, "%d", cmd_HLT);

        } else if (strcmp (s, "add") == 0)
        {
            fprintf (outfile, "%d\n", cmd_add);

        } else if (strcmp (s, "sub") == 0)
        {
            fprintf (outfile, "%d\n", cmd_sub);

        } else if (strcmp (s, "mul") == 0)
        {
            fprintf (outfile, "%d\n", cmd_mul);

        } else if (strcmp (s, "div") == 0)
        {
            fprintf (outfile, "%d\n", cmd_div);

        } else if (strcmp (s, "in") == 0)
        {
            fprintf (outfile, "%d\n", cmd_in);

        } else if (strcmp (s, "out") == 0)
        {
            fprintf (outfile, "%d\n", cmd_out);

        } else if (strcmp (s, "push") == 0)
        {
            int x = 0;

            if (fscanf (infile, "%d", &x) == 1)
            {
                fprintf (outfile, "%d %d\n", cmd_push, x);

            } else
            {
                char y[5] = "";
                fscanf (infile, "%s", y);
                if (strcmp (y, "ax") == 0)
                {
                    fprintf (outfile, "%d %d\n", cmd_rpush, ax);
                } else if (strcmp (y, "bx") == 0)
                {
                    fprintf (outfile, "%d %d\n", cmd_rpush, bx);
                } else if (strcmp (y, "cx") == 0)
                {
                    fprintf (outfile, "%d %d\n", cmd_rpush, cx);
                } else if (strcmp (y, "dx") == 0)
                {
                    fprintf (outfile, "%d %d\n", cmd_rpush, dx);
                } else
                {
                    fprintf (outfile, "ERROR\n");
                    break;
                }

            }

        } else if (strcmp (s, "pop") == 0)
        {
            char x[5] = "";
            fscanf (infile, "%s", x);
            if (strcmp (x, "ax") == 0)
            {
                fprintf (outfile, "%d %d\n", cmd_pop, ax);
            } else if (strcmp (x, "bx") == 0)
            {
                fprintf (outfile, "%d %d\n", cmd_pop, bx);
            } else if (strcmp (x, "cx") == 0)
            {
                fprintf (outfile, "%d %d\n", cmd_pop, cx);
            } else if (strcmp (x, "dx") == 0)
            {
                fprintf (outfile, "%d %d\n", cmd_pop, dx);
            } else
            {
                fprintf (outfile, "ERROR\n");
                break;
            }

        } else if (strcmp (s, "jmp") == 0)
        {
            int newip = 0;
            fscanf (infile, "%d", &newip);
            fprintf (outfile, "%d %d\n", cmd_jmp, newip);

        } else if (strcmp (s, "jb") == 0)
        {
            int newip = 0;
            fscanf (infile, "%d", &newip);
            fprintf (outfile, "%d %d\n", cmd_jb, newip);

        } else if (strcmp (s, "jbe") == 0)
        {
            int newip = 0;
            fscanf (infile, "%d", &newip);
            fprintf (outfile, "%d %d\n", cmd_jbe, newip);

        } else if (strcmp (s, "ja") == 0)
        {
            int newip = 0;
            fscanf (infile, "%d", &newip);
            fprintf (outfile, "%d %d\n", cmd_ja, newip);

        } else if (strcmp (s, "jae") == 0)
        {
            int newip = 0;
            fscanf (infile, "%d", &newip);
            fprintf (outfile, "%d %d\n", cmd_jae, newip);

        } else if (strcmp (s, "je") == 0)
        {
            int newip = 0;
            fscanf (infile, "%d", &newip);
            fprintf (outfile, "%d %d\n", cmd_je, newip);

        } else if (strcmp (s, "jne") == 0)
        {
            int newip = 0;
            fscanf (infile, "%d", &newip);
            fprintf (outfile, "%d %d\n", cmd_jne, newip);

        } else
        {
            if (getc (infile) != EOF)
                printf ("ERROR on line %lu\n", line);

            break;
        }
    }
    fclose (outfile);
    fclose (infile);
}
