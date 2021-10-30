#include "asm.h"

char *copy_in_buffer_from_file_asm (int* amount_operations)
{
    char* buffer = NULL; 
   
    FILE* asm_file = fopen ("D:/VSC/processor/library/asm.txt", "r");
    if (asm_file != NULL)
    {
        buffer = remove_trash_and_copy_in_buffer (amount_operations, asm_file);
    }
    else 
    {
        printf ("ERROR in function : %s \n"
                "asm.txt didn't opened\n", __func__);
        return NULL;
    }

    if (ferror(asm_file))
    {
        printf ("ERROR in function : %s \n"
                "reading file falled\n", __func__);
        return NULL;
    }
    fclose (asm_file);

    return buffer;
}


int make_code_rb (Onegin *line, Labels* label, const int amount_operations)
{
    if (line == NULL)
    {
        printf ("ERROR in function : %s \n" 
                "struct line have NULL ADRESS\n", __func__);
        return -1;
    }

    if (label == NULL)
    {
        printf ("ERROR in function : %s \n" 
                "struct label have NULL ADRESS\n", __func__);
        return -1;
    }

    FILE* output_file = fopen ("D:/VSC/processor/calc/code.rb", "wb");
    
    if (output_file == NULL)
    {
        printf ("ERROR in function : %s \n"
                "code.rb didn't opened\n", __func__);
        return -1;
    }

    char lb_name [MAX_AMOUNT_SMB] = {0};

    char * code_rb = (char*) calloc (1000, 5 * sizeof (char)); /////////////////////////////////////////////////////////////////////////////////////////
    if (code_rb == NULL)
    {
        printf ("ERROR in function : %s \n" 
                "haven't memory for code_rb\n", __func__);
        return -1;
    }

    char double_str [MAX_AMOUNT_SMB] = {0};
    int ip = 0;
    int ammount_lbls = 0;
    bool not_need_reasm = 1;

    double val_dbl = 0;

    Bit_Type_Comand bit_field = {0};

    /*#define make_int                                            \
            double var_dbl = strtod (double_str, NULL);         \
            value = (int)(var_dbl * FACTOR);                        */

    #define arg_inm(void)                                       \
            printf ("arg_inm %d\n", value);                     \
            bit_field.reg = 0;                                  \
            bit_field.inm = 1;                                  \
            *((Bit_Type_Comand*)(code_rb + ip)) = bit_field;    \
            ip ++;                                              \
                                                                \
            *((int*)(code_rb + ip)) = value;                    \
            ip+=4;                                              \
                                                                \
            printf ("%d\n", ip);                                \
            continue;

    #define arg_reg(void)                                       \
            printf ("arg_reg %cX\n", reg);                      \
            bit_field.reg = 1;                                  \
            bit_field.inm = 0;                                  \
            *((Bit_Type_Comand*)(code_rb + ip)) = bit_field;    \
            ip ++;                                              \
                                                                \
            *(code_rb + ip) = reg - 'A';                        \
            ip++;                                               \
                                                                \
            printf ("%d\n", ip);                                \
            continue;                                           \


    #define arg_reg_inm(void)                                   \
            printf ("arg_reg_inm %cX + %d\n", reg, value);      \
            bit_field.reg = 1;                                  \
            bit_field.inm = 1;                                  \
            *((Bit_Type_Comand*)(code_rb + ip)) = bit_field;    \
            ip ++;                                              \
                                                                \
            *(code_rb + ip) = reg - 'A';                        \
            ip++;                                               \
                                                                \
            *((int*)(code_rb + ip)) = value;                    \
            ip+=4;                                              \
                                                                \
            printf ("%d\n", ip);                                \
            continue;

    #define arg_jump(void)                                          \
            bit_field.inm = 1;                                      \
            *((Bit_Type_Comand*)(code_rb + ip)) = bit_field;        \
            ip ++;                                                  \
                                                                    \
            if (ammount_lbls == 0)                                  \
            {                                                       \
                not_need_reasm = 0;                                 \
            }                                                       \
            for (int am = 0; am < ammount_lbls; am++)               \
            {                                                       \
                if (strncmp (label[am].name, lb_name,               \
                             label[am].str_len) == 0)               \
                {                                                   \
                    printf ("in %s write it %d with name %s\n",     \
                            lb_name, label[am].ip_save,             \
                            label[am].name);                        \
                    *((int*)(code_rb + ip)) = label[am].ip_save;    \
                    not_need_reasm = not_need_reasm * 1;            \
                    break;                                          \
                }                                                   \
                printf ("in %s dont write it %d with name %s \n",   \
                        lb_name, label[am].ip_save,                 \
                        label[am].name);                            \
                not_need_reasm = 0;                                 \
            }                                                       \
            ip += 4;                                                \
            continue;                                                       


    #define DEF_CMD(num, name, ...)                                                 \
                                                                                    \
        bit_field.cmd = num;                                                        \
                                                                                    \
        if  ((*line[i].adress == 'J') &&                                            \
             (*(#name) == 'J'))                                                     \
        {                                                                           \
            if ((strncmp (line[i].adress, #name, sizeof (#name) - 1) == 0) &&       \
                (sscanf (line[i].adress + sizeof (#name), "%s", &lb_name) == 1))    \
            {                                                                       \
                arg_jump(void)                                                      \
            }                                                                       \
        }                                                                           \
                                                                                    \
        if (((strncmp ("PUSH", line[i].adress, sizeof (#name) - 1) == 0) ||         \
             (strncmp ("POP", line[i].adress, sizeof (#name) - 1) == 0)) &&         \
             (strncmp (#name, line[i].adress, sizeof (#name) - 1) == 0))            \
        {                                                                           \
                int add_pointer = 0;                                                \
                if ((*(line[i].adress + sizeof (#name)) == '[') &&                  \
                    (*(line[i].adress + strlen (line[i].adress) - 1) == ']'))       \
                {                                                                   \
                    bit_field.ram = 1;                                              \
                    add_pointer = 1;                                                \
                }                                                                   \
                if ((sscanf (line[i].adress + sizeof (#name) + add_pointer,         \
                            "%cX + %lf", &reg, &val_dbl) == 2) &&                   \
                            (isalpha (reg)))                                        \
                {                                                                   \
                    value = val_dbl * FACTOR;                                       \
                    arg_reg_inm(void)                                               \
                }                                                                   \
                if ((sscanf (line[i].adress + sizeof (#name) + add_pointer,         \
                            "%cX", &reg) == 1) &&                                   \
                            (isalpha (reg)))                                        \
                {                                                                   \
                    arg_reg(void)                                                   \
                }                                                                   \
                if (sscanf (line[i].adress + sizeof (#name) + add_pointer,          \
                            "%lf", &val_dbl) == 1)                                  \
                {                                                                   \
                    value = val_dbl * FACTOR;                                       \
                    arg_inm(void)                                                   \
                }                                                                   \
        }                                                                           \
        if (strncmp (#name, line[i].adress, sizeof (#name) - 1) == 0)               \
        {                                                                           \
            *((Bit_Type_Comand*)(code_rb + ip)) = bit_field;                        \
            ip ++;                                                                  \
            continue;                                                               \
        }

    #define ALL_OPERAIONS                                                       \
        bit_field.ram = 0;                                                      \
        bit_field.reg = 0;                                                      \
        bit_field.inm = 0;                                                      \
                                                                                \
        elem_t value = 0;                                                       \
        char reg = 0;                                                           \
                                                                                \
        printf("%s\n", line[i].adress);                                         \
                                                                                \
        if (sscanf (line[i].adress, "$ %s :", &lb_name) == 1)                   \
        {                                                                       \
            bool exit = 0;                                                      \
            printf ("$ %s :\n", lb_name);                                           \
            for (int am = 0; am < ammount_lbls; am++)                               \
            {                                                                       \
                if (strncmp (label[am].name, lb_name,                               \
                            label[am].str_len ) == 0)                      \
                {                                                                   \
                    label[am].ip_save =   ip;                                       \
                    exit = 1;                                                       \
                    break;                                                          \
                }                                                                   \
            }                                                                       \
            if (exit == 1){continue;}                                           \
            label[ammount_lbls].name    =   line[i].adress + 2;                 \
            label[ammount_lbls].str_len =   strlen (lb_name) - 1;               \
            label[ammount_lbls].ip_save =   ip     ;                            \
            ammount_lbls++;                                                     \
            continue;                                                           \
        }                                                                       \
        if (sscanf (line[i].adress, "CALL %s", &lb_name) == 1)                  \
        {                                                                       \
            printf ("CALL %s\n", lb_name);                                      \
            bit_field.cmd = CALL;                                               \
            arg_jump(void)                                                      \
            continue;                                                           \
        }

    for (int i = 0; i < amount_operations; i++)
    {
        ALL_OPERAIONS

        #include "D:\VSC\processor\library\commands.def"
    }
    
    if (not_need_reasm == 0)
    {
        ip = 0;

        for (int i = 0; i < amount_operations; i++)
        {
            ALL_OPERAIONS

            #include "D:\VSC\processor\library\commands.def"
        }
    }

    for (int i = 0; i < AMMOUNT_LABELS; i++)
    {
        printf ("\n\n label %s has ip %d", label[i].name, label[i].ip_save);
    }

    #undef ALL_OPERATIONS

    #undef DEF_CMD

    #undef arg_inm

    #undef arg_reg

    #undef arg_reg_inm

    #undef arg_jump

    /*#undef make_int*/

    if (fwrite (code_rb, sizeof (char), ip, output_file) == 0)
    {
        printf ("ERROR in function : %s \n"
                "writing file falled\n", __func__);
        return -1;
    }

    if (ferror(output_file))
    {
        printf ("ERROR in function : %s \n"
                "writing file falled\n", __func__);
        return -1;
    }
    fclose (output_file);

    return 0;
}