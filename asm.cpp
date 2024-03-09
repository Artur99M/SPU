#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "spu.h"
#include "Onegin_lib/header/readtext.h"
#include "Onegin_lib/header/outtext.h"

struct marker
{
    char str[32];
    size_t value;
};

SPU_ERROR command_init (spu* command, marker* markers, char* str_command);
SPU_ERROR asm_commads (FILE* logfile, FILE* infile, FILE* outfile, FILE* bin, spu command);
SPU_ERROR asm_jmp     (spu* command, marker* markers, FILE* infile, FILE* outfile, FILE* logfile,
size_t size_markers, FILE* bin);
bool isstrdigit (char* str);

int main ()
{
    FILE* logfile = fopen ("asm.log", "w");

    text txt;
    readtext (&txt, "proga.txt");
    for (size_t i = 0; i < txt.sizetext; i++)
    {
        if (txt.alltext[i] == ';')
            for (; i < txt.sizetext && txt.alltext[i] != '\0'; i++)
                txt.alltext[i] = '\0';
    }
    outSRC (&txt, "asm+.txt");
    FILE* infile  = fopen ("asm+.txt", "r");
    FILE* outfile = fopen ("asm.txt" , "w");
    FILE* bin = fopen ("asm.bin", "wb");
    struct stat buff;
    stat("asm+.txt", &buff);
    size_t size_asm = (size_t) buff.st_size;

    marker* markers = (marker*) calloc (4, sizeof (marker));
    if (markers == nullptr)
    {
        fputs ("Array for markers wasn't made\n", logfile);
        abort();
    }
    size_t size_markers = 0,
       capacity_markers = 4;


    for (size_t ip = 0; ftell(infile) < size_asm; ip++)
    {
        char s[256] = "";
        fscanf (infile, "%s", s);

        short i = 0;
        for (; i < 19; i++)
        {
            if (strcmp (cmds[i].str, s) == 0)
                break;
        }
        for (short j; j < 4; j++)
        {
            if (strcmp (registers[j].str, s) == 0)
                i = j;
        }
        if (i == 19 && !isstrdigit(s))
        {
            strcpy (markers[size_markers].str, s);
            markers[size_markers].value = (ip--);
            size_markers++;
        }

        if (size_markers >= capacity_markers)
            if ((markers = (marker*) realloc (markers,
            (capacity_markers *= 2) * sizeof (marker))) == nullptr)
            {
                fputs ("ERROR: markers ERROR", logfile);
                abort();
            }
    }


    if (fseek (infile, 0, SEEK_SET) != 0)
    {
        fputs ("ERROR when markers were read\n", logfile);
        abort();
    }

    for (size_t line = 0; ftell(infile) != size_asm; line++)
    {
        spu command = cmd_HLT;
        char str_command[32] = "";
        fscanf (infile, "%s", str_command);
        if (command_init (&command, markers, str_command) == SPU_NO_ERROR)
            if ((int) command < 9 || command == cmd_ret)
                asm_commads (logfile, infile, outfile, bin, command);
            if (command == dir_org)
            {
                size_t q = 0;
                if (fscanf (infile, "%lu", &q) == 0)
                {
                    fputs ("ORG ERROR\n", logfile);
                    abort();
                }
                for (; q > 0; q--)
                {
                    fprintf (outfile, "%d\n", cmd_HLT);
                    fwrite (
            }
            if (9 <= (int) command && (int) command < 17)
                asm_jmp (&command, markers, infile, outfile, logfile, size_markers, bin);

    }

    fclose (bin);
    fclose (outfile);
    fclose (infile);
    fclose (logfile);
}

SPU_ERROR command_init (spu* command, marker* markers, char* str_command)
{
    if (command == nullptr) return SPU_NO_COMMAND_ERROR;

    int i = 0;
    for (; i < 19 && strcmp (cmds[i].str, str_command); i++);

    if (i == 19) return SPU_MARKER;
    *command = cmds[i].cmd;

    return SPU_NO_ERROR;
}
SPU_ERROR asm_commads (FILE* logfile, FILE* infile, FILE* outfile, FILE* bin, spu command)
{
    if ((int) command < 7 || command == cmd_ret)
        fprintf (outfile, "%d\n", command);

    else if (command == cmd_push)
    {
        long start_ftell = ftell (infile);
        int elem = 0;
        if (fscanf (infile, "%d", &elem) == 1)
            fprintf (outfile, "%d %d\n", 0x200|cmd_push, elem);
        else
        {
            if (fseek (infile, start_ftell, SEEK_SET) != 0)
            {
                fputs ("ERROR: fseek in push ERROR\n", logfile);
                abort();
            }
            char text[12] = "";
            fscanf (infile, "%s", text);
            int i = 0;
            for (; i < 4; i++)
                if (strcmp (registers[i].str, text) == 0)
                {
                    fprintf (outfile, "%d %d\n", 0x400|cmd_push, registers[i].value);
                    i--;
                    break;
                }
            if (i == 4)
            {
                if (fseek (infile, start_ftell, SEEK_SET) != 0)
                    {
                        fputs ("ERROR: fseek in push ERROR\n", logfile);
                        abort();
                    }
                char c = '\0';
                while ((c = getc (infile)) != '[' && c != '\n');
                if (c == '[')
                {
                    if (fscanf (infile, "%d", &elem) == 1)
                    {
                        fprintf (outfile, "%d %d\n", 0x800|cmd_push, elem);
                        while ((c = getc (infile)) != '\n' && c != ' ');
                    }
                    else
                    {
                        fputs ("ERROR: push without addres\n", logfile);
                        abort();
                    }
                }
            }
        }
    }
    else if (command == cmd_pop)
    {
        long start_ftell = ftell (infile);
        int elem = 0;
        char text[12] = "";
        fscanf (infile, "%s", text);
        int i = 0;
        for (; i < 4; i++)
            if (strcmp (registers[i].str, text) == 0)
            {
                fprintf (outfile, "%d %d\n", 0x400|cmd_pop, registers[i].value);
                i--;
                break;
            }
        if (i == 4)
        {
            if (fseek (infile, start_ftell, SEEK_SET) != 0)
                {
                    fputs ("ERROR: fseek in pop ERROR\n", logfile);
                    abort();
                }
            char c = '\0';
            while ((c = getc (infile)) != '[' && c != '\n');
            if (c == '[')
            {
                if (fscanf (infile, "%d", &elem) == 1)
                {
                    fprintf (outfile, "%d %d\n", 0x800|cmd_pop, elem);
                    while ((c = getc (infile)) != '\n' && c != ' ');
                }
                else
                {
                    fputs ("ERROR: pop without addres\n", logfile);
                    abort();

                }
            }
        }
    }
    return SPU_NO_ERROR;
}

bool isstrdigit (char* str)
{
    if (str == nullptr) return false;

    bool isdgt = true;
    for (; *str != '\0'; str++)
        if (isdigit (*str) == 0) return false;

    return true;
}

SPU_ERROR asm_jmp     (spu* command, marker* markers, FILE* infile, FILE* outfile, FILE* logfile,
size_t size_markers, FILE* bin)
{
    if (command == nullptr || markers == nullptr || infile == nullptr ||
    outfile == nullptr || logfile == nullptr) return SPU_NULLPTR;

    size_t addres = 0;
    if (fscanf (infile, "%lu", &addres) == 1)
    {
        fprintf (outfile, "%d %lu\n", *command, addres);
        return SPU_NO_ERROR;
    }

    char mark[32] = "";
    if (fscanf (infile, "%s", mark) == 0)
        return SPU_NO_MARKER_ERROR;

    mark[strlen(mark)] = ':';
    for (size_t i = 0; i < size_markers; i++)
        if (strcmp (markers[i].str, mark) == 0)
        {
            fprintf (outfile, "%d %lu\n", *command, markers[i].value);
            return SPU_NO_ERROR;
        }

    return SPU_NO_MARKER_ERROR;
}
