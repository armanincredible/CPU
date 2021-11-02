#ifndef CALCHEADER
#define CALCHEADER

#include "..\library\Onegin\dora.h"
#include "..\library\Stack\stack.h"
#include "sys\stat.h"
#include "..\library\headers\codes_operations.h"

struct Proc_str
{
    struct Stack st;
    struct Stack rtn;
    int Ip = 0;
    elem_t* Regs;
    char* Code;
    elem_t* Ram;
};

struct BITS
{
    unsigned fr : 1;
    unsigned sec : 1;
};

const int SIZE_RAM = 100;
const int AMMOUNT_REGS = 10;
const int AMMOUNT_SAVES_IP = 10;

int take_arguments (const char * array, int* ip, int * reg_value, elem_t* inm_value, Bit_Type_Comand* bit_field);

elem_t calculate (Stack* stack, Proc_str* proc);

int proc (void);

char* make_array (FILE* input_file);

#endif