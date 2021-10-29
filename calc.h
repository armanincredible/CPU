#ifndef CALCHEADER
#define CALCHEADER

#include "stack.h"
#include "dora.h"
#include "sys\stat.h"
#include "codes_operations.h"

struct Proc_str
{
    struct Stack st;
    struct Stack rtn;
    int Ip = 0;
    int* Regs;
    char* Code;
    elem_t* Ram;
};

const int FACTOR = 100;
const int AMMOUNT_REGS = 4;

int take_arguments (const char * array, int* ip, int * reg_value, int* inm_value, Bit_Type_Comand* bit_field);

elem_t calculate (Stack* stack, Stack* rtr_stack, Proc_str* proc);

int proc (void);

char* make_array (FILE* input_file);

#endif