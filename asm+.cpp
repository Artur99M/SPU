#include <stdio.h>
#include <string.h>
#include "spu.h"
#include <stdlib.h>
#include <sys/stat.h>


int main ()
{
    FILE* infile = fopen ("proga.txt", "r");
    FILE* outfile = fopen ("asm.txt", "w");
    FILE* logfile = fopen ("asm.log", "w");

    struct stat buff;
    stat ("proga.txt", &buff);
    size_t size_infile = buff.st_size;

    for (size_t line = 0; ftell(infile) != infile; line++)
    {
        char s[32] = "";
        fscanf (infile, "%s", s);
        for (size_t i = 0; i < 19; i++)
        {
            if (strcmp(spaceSPU::commands[i], s) == 0)
            {
                if (i < 7 || i == 18)
                {
                    fprintf (outfile, "%d\n", spaceSPU::cmds[i]);
                    break;
                }
                else if
