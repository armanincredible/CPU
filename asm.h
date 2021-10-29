#ifndef ASMHEADER
#define ASMHEADER

#include "dora.h"
#include "stack.h"
#include "codes_operations.h"

const int AMMOUNT_LABELS = 10;
const int FACTOR = 100;
const int MAX_AMOUNT_SMB = 20;

struct Labels
{
       int ip_save;
       int str_len;
       char* name;
};

int make_code_rb (Onegin *line, Labels* label, const int amount_operations);

char *copy_in_buffer_from_file_asm (int* amount_operations);

#endif