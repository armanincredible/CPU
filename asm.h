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

int make_code_rb (Onegin *line, Labels* label, const int amount_operations);

char *copy_in_buffer_from_file_asm (int* amount_operations);

int parsing (const char* name_of_operaion, char* line_operation, char* lb_name, Bit_Type_Comand* bit_field, char * code_rb, int* ip,
             int* ammount_lbls, Labels* label, const int amount_operations, int* cur_operation, bool* not_need_reasm, int* am_asm, const int size_name);

#endif