#ifndef DISASMHEADER
#define DISASMHEADER

#include "D:\VSC\processor\library\dora.h"
#include "D:\VSC\processor\library\stack.h"
#include "D:\VSC\processor\library\codes_operations.h"

char* make_array (FILE* input_file);

int write_file (FILE* input_file, FILE* output_file, char* array_rb);

int take_arguments (const char * array, int* ip, int * reg_value, int* inm_value, Bit_Type_Comand* bit_field);

#endif