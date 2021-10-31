#include "disasm.h"

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
    if (input_file == NULL)
    {
        printf ("ERROR in function : %s \n"
                "input_file have NULL adress\n", __func__);
        return NULL;
    }

    if (output_file == NULL)
    {
        printf ("ERROR in function : %s \n"
                "input_file have NULL adress\n", __func__);
        return NULL;
    }

    if (array_rb == NULL)
    {
        printf ("ERROR in function : %s \n"
                "haven't memory for array\n", __func__);
        return NULL;
    }

    int ammount = 0;
    int operation = 0;
            
    #define DEF_CMD(num, name, ...)                                     \
            {                                                           \
                case name :                                             \
                {                                                       \
                    if (num == CALL)                                    \
                    {                                                   \
                        fprintf (output_file, "$ ");                    \
                    }                                                   \
                    fprintf (output_file, "%s ", #name);                \
                    if (bit_field.ram == 1)                             \
                    {                                                   \
                        fprintf (output_file, "[ ");                    \
                    }                                                   \
                    if (bit_field.reg == 1)                             \
                    {                                                   \
                        fprintf (output_file, "%cX ", reg_value + 'A'); \
                    }                                                   \
                    if (bit_field.inm == 1)                             \
                    {                                                   \
                        if ((num != PUSH) && (num != POP))              \
                        {                                               \
                            fprintf (output_file, "%d",                 \
                                     inm_value);                        \
                        }                                               \
                        else                                            \
                        {                                               \
                            fprintf (output_file, "%lf",                \
                                    (double (inm_value)) / FACTOR);     \
                        }                                               \
                    }                                                   \
                    if (bit_field.inm == 1 && bit_field.reg == 1)       \
                    {                                                   \
                        fprintf (output_file, " + ");                   \
                    }                                                   \
                    if (bit_field.ram == 1)                             \
                    {                                                   \
                        fprintf (output_file, "]");                     \
                    }                                                   \
                    ammount++;                                          \
                    fprintf (output_file, "\n");                        \
                    break;                                              \
                }                                                       \
            }
    Bit_Type_Comand bit_field = {0};
    int ip = 0;
    int reg_value = 0;
    int inm_value = 0;

    int size_input_file = size_file (input_file);

    while (ip < size_input_file)
    {   
        bit_field = *((Bit_Type_Comand*)(array_rb + ip));
        take_arguments (array_rb, &ip, &reg_value, &inm_value, &bit_field);

        operation = bit_field.cmd;
        switch (operation)
        {
            #include "D:\VSC\processor\library\commands.def"
        }
    }

    #undef DEF_CMD

    return 0;
}

int take_arguments (const char * array, int* ip, int * reg_value, int* inm_value, Bit_Type_Comand* bit_field)
{
    if (array == NULL)
    {
        printf ("ERROR in function : %s \n"
                "array have NULL adress\n", __func__);
        return -1;
    }

    if (ip == NULL)
    {
        printf ("ERROR in function : %s \n"
                "ip have NULL adress\n", __func__);
        return -1;
    }

    if (reg_value == NULL)
    {
        printf ("ERROR in function : %s \n"
                "reg_value have NULL adress\n", __func__);
        return -1;
    }

    if (inm_value == NULL)
    {
        printf ("ERROR in function : %s \n"
                "inm_value have NULL adress\n", __func__);
        return -1;
    }

    if (bit_field == NULL)
    {
        printf ("ERROR in function : %s \n"
                "bit_field have NULL adress\n", __func__);
        return -1;
    }


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
            *inm_value = *((int*)(array + *ip + 2));
            *ip += 6;
            return 0;
        }
        *ip += 2;
    }
    else if (bit_field -> inm == 1)
    {
        *inm_value = *((int*)(array + *ip + 1));
        *ip += 5;
    }
    return 0;
}

