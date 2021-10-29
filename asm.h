#ifndef ASMHEADER
#define ASMHEADER

#include "dora.h"
#include "stack.h"
#include "codes_operations.h"

const int amount_labels = 10;

struct Labels
{
       int ip_save;
       char* name;
};

// enum CommandsNum {
       //startRegNum = -100,
//     // #define DEF_CMD(name, ...) name,
//     // #include commands.def
//     // #undef DEF_CMD

//     // PUSH,
//     // POP,


// };



int make_code_rb (Onegin *line, Labels* label, const int amount_operations);

char *copy_in_buffer_from_file_asm (int* amount_operations);

#endif