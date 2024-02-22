#include <stdio.h>
#include "spu.h"
#include <sys/stat.h>


int main ()
{
    FILE* infile  = fopen ("asm.txt", "r");
    FILE* outfile = fopen ("disasm.txt", "w");
    FILE* logfile = fopen ("disasm.log", "w");

    struct stat buff;
    stat ("asm.txt", &buff);
    size_t size_infile = buff.st_size;

    int end = 0, x = 0, elem = 0;

    for (size_t line = 1; end == 0; line++)
    {
        if (fscanf (infile, "%d", &x) == 0 || ftell(infile) == size_infile)
            if (ftell(infile) == size_infile) end = 1;
            else
            {
                fprintf (logfile, "disasm error on line %lu\n", line);
                end = 1;
            }

        else
        {
            elem_t elem = 0;
            for (int i = 0; i < 20; i++)
            {

                if (x == (int) spaceSPU::cmds[i])
                {
                    if (i < 7 || i == 18)
                    {
                        fprintf (outfile, "%s\n", spaceSPU::commands[i]);
                        break;
                    }
                    else if (i == 7 || (9 < i && i < 18))
                    {
                        if (fscanf (infile, "%d", &elem) == 0)
                        {
                            fprintf (logfile, "disasm error on line %lu\n", line);
                            end = 1;
                        }
                        else
                            fprintf (outfile, "%s %d\n", spaceSPU::commands[i], elem);
                        break;
                    }
                    else if (i == 8 || i == 9)
                    {
                        if (fscanf (infile, "%d", &elem) == 0)
                        {
                            fprintf (logfile, "disasm error on line %lu\n", line);
                            end = 1;
                        }
                        else
                            fprintf (outfile, "%s %s\n", spaceSPU::commands[i], spaceSPU::registers[elem]);
                        break;
                    }
                    else
                    {
                        fprintf (logfile, "disasm error on line %lu\n", line);
                        end = 1;
                        break;
                    }
                }
            }
        }
    }

    fclose (logfile);
    fclose (infile);
    fclose (outfile);
}
