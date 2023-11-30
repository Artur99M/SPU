#include <stdio.h>
#include <string.h>
#include "spu.hpp"
//HLT = -1, push = 1, add = 2, sub = 3, mul = 4, divi = 5, out = 6, in = 7,

int main ()
{
    FILE* infile = fopen ("proga.txt", "r");
    FILE* outfile = fopen ("asm.txt", "w");

    forever
    {
        char s[12] = "";
        fscanf (infile, "%s", s);

        if (strcmp (s, "HLT") == 0)
        {
            fprintf (outfile, "%d", HLT);

        } else if (strcmp (s, "add") == 0)
        {
            fprintf (outfile, "%d\n", add);

        } else if (strcmp (s, "sub") == 0)
        {
            fprintf (outfile, "%d\n", sub);

        } else if (strcmp (s, "mul") == 0)
        {
            fprintf (outfile, "%d\n", mul);

        } else if (strcmp (s, "divi") == 0)
        {
            fprintf (outfile, "%d\n", divi);

        } else if (strcmp (s, "in") == 0)
        {
            fprintf (outfile, "%d\n", in);

        } else if (strcmp (s, "out") == 0)
        {
            fprintf (outfile, "%d\n", out);

        } else if (strcmp (s, "push") == 0)
        {
            int x = 0;

            if (fscanf (infile, "%d", &x) == 1)
            {
                fprintf (outfile, "%d %d\n", push, x);

            } else
            {
                char y[5] = "";
                fscanf (infile, "%s", y);
                if (strcmp (y, "ax") == 0)
                {
                    fprintf (outfile, "%d %d\n", rpush, ax);
                } else if (strcmp (y, "bx") == 0)
                {
                    fprintf (outfile, "%d %d\n", rpush, bx);
                } else if (strcmp (y, "cx") == 0)
                {
                    fprintf (outfile, "%d %d\n", rpush, cx);
                } else if (strcmp (y, "dx") == 0)
                {
                    fprintf (outfile, "%d %d\n", rpush, dx);
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
                fprintf (outfile, "%d %d\n", pop, ax);
            } else if (strcmp (x, "bx") == 0)
            {
                fprintf (outfile, "%d %d\n", pop, bx);
            } else if (strcmp (x, "cx") == 0)
            {
                fprintf (outfile, "%d %d\n", pop, cx);
            } else if (strcmp (x, "dx") == 0)
            {
                fprintf (outfile, "%d %d\n", pop, dx);
            } else
            {
                fprintf (outfile, "ERROR\n");
                break;
            }
        } else
        {
            break;
        }
    }
    fclose (outfile);
    fclose (infile);
}
