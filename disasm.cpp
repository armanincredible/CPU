#include "disasm.h"

#define cmp_zero_adr(name)                                  \
    if (name == NULL)                                       \
    {                                                       \
        printf ("\nERROR in function : %s \n"               \
                "%s have NULL ADRESS\n", #name, __func__);  \
        return -1;                                          \
    }

#define status_work_break                                   \
        if (status_work == -1)                              \
        {                                                   \
            break;                                          \
        }

char* make_array (FILE* input_file)
{
    if (input_file == NULL)
    {
        printf ("ERROR in function : %s \n"
                "input_file have NULL adress\n", __func__);
        return NULL;
    }

    int size = size_file (input_file);////////////////////////////////SIZE_TTTTTTTTTT

    char* array = (char*) calloc (size, sizeof(char));
    if (array == NULL)
    {
        printf ("ERROR in function : %s \n"
                "haven't memory for array\n", __func__);
        return NULL;
    }

    fread (array, sizeof (char), size, input_file);

    return array;
}

int write_file (FILE* input_file, FILE* output_file, char* array_rb)
{
    cmp_zero_adr(input_file)

    cmp_zero_adr(output_file)

    cmp_zero_adr(array_rb)

    int ammount = 0;
    int operation = 0;
    int status_work = 0;
            
    Bit_Type_Comand bit_field = {0, 0, 0, 0};   

    int ip = 0;
    int reg_value = 0;
    elem_t inm_value = 0;

    int size_input_file = size_file (input_file);

    while (ip < size_input_file)
    {   
        bit_field = *((Bit_Type_Comand*)(array_rb + ip));
        take_arguments (array_rb, &ip, &reg_value, &inm_value, &bit_field);

        status_work_break

        operation = bit_field.cmd;
        switch (operation)
        {
            #define DEF_CMD(num, name, ...)                                             \
                {                                                                       \
                    case name :                                                         \
                    {                                                                   \
                        status_work  = output_pars (&bit_field, output_file, num,       \
                                                    #name, &reg_value, &inm_value);     \
                        ammount++;                                                      \
                        fprintf (output_file, "\n");                                    \
                        break;                                                          \
                    }                                                                   \
                }
            #include "..\library\headers\commands.def"
            #undef DEF_CMD
        }

        status_work_break
    }

    return 0;
}

int take_arguments (const char * array, int* ip, int * reg_value, elem_t* inm_value, Bit_Type_Comand* bit_field)
{
    cmp_zero_adr(array)

    cmp_zero_adr(ip)

    cmp_zero_adr(reg_value)

    cmp_zero_adr(inm_value)

    cmp_zero_adr(bit_field)

    if ((bit_field->reg == 0) && (bit_field->inm == 0))
    {
        *ip += 1;
        return 0;
    }
    if (bit_field->reg == 1)
    {
        *reg_value = array [*ip + 1];
        if (bit_field -> inm == 1)
        {
            *inm_value = *((elem_t*)(array + *ip + 2));
            *ip += 2 + sizeof (elem_t);
            return 0;
        }
        *ip += 2;
    }
    else if (bit_field -> inm == 1)
    {
        *inm_value = *((elem_t*)(array + *ip + 1));
        *ip += 1 + sizeof (elem_t);
    }
    return 0;
}


int output_pars (Bit_Type_Comand* bit_field, FILE* output_file, const int num, const char* name, int* reg_value, elem_t* inm_value)
{
    cmp_zero_adr(bit_field)
    cmp_zero_adr(output_file)
    cmp_zero_adr(name)
    cmp_zero_adr(reg_value)
    cmp_zero_adr(inm_value)

    if (num == CALL)
    {
        fprintf (output_file, "$ ");
    }
    fprintf (output_file, "%s ", name);
    if (bit_field->ram == 1)
    {
        fprintf (output_file, "[ ");
    }
    if (bit_field->reg == 1)
    {
        fprintf (output_file, "%cX ", *reg_value + 'A');
    }
    if (bit_field->inm == 1)
    {
        if ((num != PUSH) && (num != POP))
        {
            fprintf (output_file, "%lf",
                        *inm_value);
        }
        else                                            
        {                                              
            fprintf (output_file, "%lf",               
                    (double (*inm_value)) / FACTOR);    
        }                                               
    }                                                  
    if (bit_field->inm == 1 && bit_field->reg == 1)      
    {                                                  
        fprintf (output_file, " + ");                   
    }                                                   
    if (bit_field->ram == 1)                             
    {                                                   
        fprintf (output_file, "]");                    
    }

    return 0;
}

#undef status_work_break    

#undef cmp_zero_adr 