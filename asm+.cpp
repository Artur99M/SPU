#include <stdio.h>
#include <string.h>
#include "spu.h"
#include <stdlib.h>
#include <sys/stat.h>
#include "Onegin/header/readtext.h"
#include "Onegin/header/outtext.h"
#include <ctype.h>

struct marker
{
    char str[32];
    size_t value;
};

SPU_ERROR command_init (spu* command, marker* markers, char* str_command);
SPU_ERROR asm_commads (FILE* logfile, FILE* infile, FILE* outfile, spu command);

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
    puts (txt.line[0].str);

    marker* markers = (marker*) calloc (4, sizeof (marker));
    if (markers == nullptr)
    {
        fputs ("Array for markers wasn't made\n", logfile);
        abort();
    }
    size_t size_markers = 0, capacity_markers = 4;

    printf ("%lu\n", txt.numlines);

    for (size_t ip = 0; ip < txt.numlines; ip++)
    {
        printf ("%lu\n", ip);
        puts (txt.line[ip].str);
        printf ("%mark size = %lu\n", size_markers);
        short i = 0;
        for (; i < 19; i++)
        {
            if (strcmp (cmds[i].str, txt.line[ip].str) == 0)
                break;
        }
        for (short j; j < 4; j++)
        {
            if (strcmp (cmds[j].str, txt.line[ip].str) == 0)
                i = j;
        }
        printf ("i = %d\n", i);
        if (i == 19 && !isdigit(txt.line[ip].str[0]))
        {
            strcpy (markers[size_markers].str, txt.line[ip].str);
            markers[size_markers].value = ip - 1;
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

    puts (markers[0].str);
//     for (size_t i = 0; i < size_markers; i++)
//         printf ("%s, %lu\n", markers[i].str, markers[i].value);
//
//
//     if (fseek (infile, 0, SEEK_SET) != 0)
//     {
//         fputs ("ERROR when markers were read\n", logfile);
//         abort();
//     }
//
//     for (size_t line = 0; ftell(infile) != size_infile; line++)
//     {
//         spu command = cmd_HLT;
//         char str_command[32] = "";
//         fscanf (infile, "%s", str_command);
//         if (command_init (&command, &markers, str_command) == SPU_NO_ERROR)
//             if ((int) command < 9 || command == cmd_ret)
//                 asm_commads (logfile, infile, outfile, command);
//
//     }
//
     fclose (logfile);
}

// SPU_ERROR command_init (spu* command, marker* markers, char* str_command)
// {
//     if (command == nullptr) return SPU_NO_COMMAND_ERROR;
//
//     int i = 0;
//     for (; i < 19 && strcmp (cmds[i].str, str_command); i++);
//
//     if (i == 19) return SPU_MARKER;
//     *command = cmds[i].cmd;
//
//     return SPU_NO_ERROR;
// }
// SPU_ERROR asm_commads (FILE* logfile, FILE* infile, FILE* outfile, spu command)
// {
//     if ((int) command < 7 || command == cmd_ret)
//         fprintf (outfile, "%d\n", command);
//
//     else if (command == cmd_push)
//     {
//         long start_ftell = ftell (infile);
//         int elem = 0;
//         if (fscanf (infile, "%d", &elem) == 1)
//             fprintf (outfile, "%d %d\n", 0x20|cmd_push, elem);
//         else
//         {
//             char text[12] = "";
//             fscanf (infile, "%s", text);
//             int i = 0;
//             for (; i < 4; i++)
//                 if (strcmp (registers[i].str, text) == 0)
//                 {
//                     fprintf (outfile, "%d %d\n", 0x40|cmd_push, registers[i].value);
//                     i--;
//                     break;
//                 }
//             if (i == 3)
//             {
//                 if (fseek (infile, start_ftell, SEEK_CUR) != 0)
//                     {
//                         fputs ("ERROR: fseek in push ERROR\n", logfile);
//                         abort();
//                     }
//                 char c = '\0';
//                 while ((c = getc (infile)) != '[' && c != '\n' && c != ' ');
//                 if (c == '[')
//                 {
//                     if (fscanf (infile, "%d", &elem) == 1)
//                     {
//                         fprintf (outfile, "%d %d", 0x80|cmd_push, elem);
//                         while ((c = getc (infile)) != '\n' && c != ' ');
//                     }
//                     else
//                     {
//                         fputs ("ERROR: push without addres\n", logfile);
//                         abort();
//                     }
//                 }
//             }
//         }
//     /////////////////////////////////
//     }
//     else if (command == cmd_pop)
//     {
//         long start_ftell = ftell (infile);
//         int elem = 0;
//         char text[12] = "";
//         fscanf (infile, "%s", text);
//         int i = 0;
//         for (; i < 4; i++)
//             if (strcmp (registers[i].str, text) == 0)
//             {
//                 fprintf (outfile, "%d %d\n", 0x40|cmd_pop, registers[i].value);
//                 i--;
//                 break;
//             }
//         if (i == 3)
//         {
//             if (fseek (infile, start_ftell, SEEK_CUR) != 0)
//                 {
//                     fputs ("ERROR: fseek in pop ERROR\n", logfile);
//                     abort();
//                 }
//             char c = '\0';
//             while ((c = getc (infile)) != '[' && c != '\n' && c != ' ');
//             if (c == '[')
//             {
//                 if (fscanf (infile, "%d", &elem) == 1)
//                 {
//                     fprintf (outfile, "%d %d", 0x80|cmd_pop, elem);
//                     while ((c = getc (infile)) != '\n' && c != ' ');
//                 }
//                 else
//                 {
//                     fputs ("ERROR: pop without addres\n", logfile);
//                     abort();
//                     }
//                 }
//             }
//         }
//     }
// }
//
