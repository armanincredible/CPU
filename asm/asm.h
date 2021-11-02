#ifndef ASMHEADER
#define ASMHEADER

#include "..\library\Onegin\dora.h"
#include "..\library\Stack\stack.h"
#include "..\library\headers\codes_operations.h"

const int AMMOUNT_LABELS = 50;
const int MAX_AMOUNT_SMB = 20;

struct Labels
{
       int ip_save;
       int str_len;
       char* name;
};

struct Global 
{
       char* code_rb;
       int ip;
       int ammount_lbls;
       int not_need_reasm;
       int size_name;
       int am_asm;
       const char* name_of_operaion;
       char* line_operation;
       int cur_operation;
       int amount_operations;
};

int make_code_rb (Onegin *line, Labels* label, const int amount_operations);

char *copy_in_buffer_from_file_asm (int* amount_operations);

int parsing_push_pop (Global* global, Bit_Type_Comand* bit_field,  Labels* label);

int parsing (Global* global, Bit_Type_Comand* bit_field,  Labels* label);

int parsing_call_jump (Global* global, Bit_Type_Comand* bit_field,  Labels* label);

int parsing_default (Global* global, Bit_Type_Comand* bit_field,  Labels* label);

#endif