#ifndef DISASMHEADER
#define DISASMHEADER

#include "..\library\Onegin\dora.h"
#include "..\library\Stack\stack.h"
#include "..\library\headers\codes_operations.h"

char* make_array (FILE* input_file);

int write_file (FILE* input_file, FILE* output_file, char* array_rb);

int take_arguments (const char * array, int* ip, int * reg_value, elem_t* inm_value, Bit_Type_Comand* bit_field);

int output_pars (Bit_Type_Comand* bit_field, FILE* output_file, const int num, const char* name, int* reg_value, elem_t* inm_value);

#endif