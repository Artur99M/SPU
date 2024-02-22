#include <stdio.h>
#include <string.h>
#include "spu.h"
#include <stdlib.h>


int push_asm (FILE* infile, FILE* outfile);
int pop_asm (FILE* infile, FILE* outfile);

int main ()
{
    FILE* infile = fopen ("proga.txt", "r");
    FILE* outfile = fopen ("asm.txt", "w");
    FILE* logfile = fopen ("asm.log", "w");
    int end = 0;
    for (size_t line = 1, ip = 0; end == 0; line++, ip++)
    {
        char s[12] = "";
        fscanf (infile, "%s", s);
        for (int i = 0; i < 21; i++)
        {
            if (i == 20)
            {
                if (getc (infile) != EOF)
                    fprintf (logfile, "asm: ERROR on line %lu\n", line);
                end = 1;
                break;
            }

            if (strcmp (spaceSPU::commands[i], s) == 0)
            {
                if (i < 7 || i == 18)
                {
                    fprintf (outfile, "%d\n", spaceSPU::cmds[i]);
                    break;
                }
                else if (i == 7)
                {
                    if ((end = push_asm (infile, outfile)) == 1)
                        fprintf (logfile, "PUSH ERROR (wrong reg) on line %lu!\n", line);
                    ip++;
                    break;
                }
                else if (i == 9)
                {
                    if ((end = pop_asm (infile, outfile)) == 1)
                        fprintf (logfile, "POP ERROR (wrong reg) on line %lu!\n", line);
                    ip++;
                    break;
                }
                else if (9 < i && i < 17)
                {
                    int newip = 0;
                    if (fscanf (infile, "%d", &newip) == 0)
                    {
                        fprintf (logfile, "asm: JUMP ERROR on line %lu!\n", line);
                        end = 1;
                    }
                    else
                    {
                        fprintf (outfile, "%d %d\n", spaceSPU::cmds[i], newip);
                    }
                    ip++;
                    break;
                }
                else if (i == 19)
                {
                    size_t pointer = 0;
                    if (fscanf (infile, "%lu", &pointer) == 0 || pointer < 0)
                    {
                        fprintf (logfile, "asm: ORG ERROR on line %lu!\n", line);
                        end = 1;
                    }
                    else
                    {
                        for (;ip < pointer; ip++)
                            fprintf (outfile, "%d\n", cmd_HLT);
                    }
                    break;
                }
                else if (i == 17)
                {
                    size_t pointer = 0;
                    if (fscanf (infile, "%lu", &pointer) == 0 || pointer < 0)
                    {
                        fprintf (logfile, "asm: CALL ERROR on line %lu!\n", line);
                        end = 1;
                    }
                    else
                    {
                        fprintf (outfile, "%d %lu\n", cmd_call, pointer);
                    }
                    ip++;
                    break;
                }
                else
                {
                    fprintf (logfile, "asm: ERROR on line %lu!\n", line);
                    end = 1;
                    break;
                }
            }
        }
    }
    fclose (logfile);
    fclose (outfile);
    fclose (infile);
}


int push_asm (FILE* infile, FILE* outfile)
{
    int x = 0;

    if (fscanf (infile, "%d", &x) == 1)
    {
        fprintf (outfile, "%d %d\n", cmd_push, x);

    } else
    {
        char y[5] = "";
        fscanf (infile, "%s", y);
        for (int j = 0; j < 5; j++)
        {
            if (j == 5)
            {
                return 1;
                break;
            }
            else if (strcmp(spaceSPU::registers[j], y) == 0)
            {
                fprintf (outfile, "%d %d\n", cmd_rpush, j);
                break;
            }
        }
    }
    return 0;
}

int pop_asm (FILE* infile, FILE* outfile)
{
    char x[5] = "";
    fscanf (infile, "%s", x);
    for (int j = 0; j < 5; j++)
    {
        if (j == 5)
        {
            return 1;
            break;
        }
        else if (strcmp(spaceSPU::registers[j], x) == 0)
        {
            fprintf (outfile, "%d %d\n", cmd_pop, j);
            break;
        }
    }
    return 0;
}

