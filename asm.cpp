#include "asm.h"

char *copy_in_buffer_from_file_asm (int* amount_operations)
{
    char* buffer = NULL; 
    ///////////ASSERT AMOUNT_OPERATONS
    FILE* asm_file = fopen ("asm.txt", "r");
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
                "haven't memory for struct line\n", __func__);
        return -1;
    }

    if (label == NULL)
    {
        printf ("ERROR in function : %s \n" 
                "haven't memory for struct line\n", __func__);
        return -1;
    }

    FILE* output_file = fopen ("code.rb", "wb");
    
    if (output_file == NULL)
    {
        printf ("ERROR in function : %s \n"
                "code.rb didn't opened\n", __func__);
        return -1;
    }

    char lb_name [1000] = {0};
    char code_rb [1000] = {0};
    int ip = 0;
    int ammount_lbls = 0;

    Bit_Type_Comand bit_field = {0};

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
            *((Bit_Type_Comand*)(code_rb + ip)) = bit_field;        \
            ip ++;                                                  \
                                                                    \
            for (int am = 0; am <= ammount_lbls; am++)              \
            {                                                       \
                if (strcmp (label[am].name, lb_name) == 0)          \
                {                                                   \
                    *((int*)(code_rb + ip)) = label[am].ip_save;    \
                    ip+=4;                                          \
                    break;                                          \
                }                                                   \
            }                                                       \
            continue;                                               
    

    for (int i = 0; i < amount_operations; i++)
    {
        bit_field.ram = 0;
        bit_field.reg = 0;
        bit_field.inm = 0;

        elem_t value = 0;
        char reg = 0;

        printf("%s\n", line[i].adress);

        if (sscanf (line[i].adress, ": %s", &lb_name) == 1)
        {
            printf (": %s\n", lb_name);
            label[ammount_lbls].name    =   lb_name;
            label[ammount_lbls].ip_save =   ip     ;
            continue;
        }

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
                    bit_field.inm = 1;                                                  \
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
                    if (sscanf (line[i].adress + sizeof (#name) + add_pointer,          \
                                "%cX + %d", &reg, &value) == 2)                         \
                    {                                                                   \
                            arg_reg_inm(void)                                           \
                    }                                                                   \
                    if (sscanf (line[i].adress + sizeof (#name) + add_pointer,          \
                                "%d", &value) == 1)                                     \
                    {                                                                   \
                            arg_inm(void)                                               \
                    }                                                                   \
                    if (sscanf (line[i].adress + sizeof (#name) + add_pointer,          \
                                "%c", &reg) == 1)                                       \
                    {                                                                   \
                            arg_reg(void)                                               \
                    }                                                                   \
            }                                                                           \
            if (strncmp (#name, line[i].adress, sizeof (#name) - 1) == 0)               \
            {                                                                           \
                *((Bit_Type_Comand*)(code_rb + ip)) = bit_field;                        \
                ip ++;                                                                  \
                                                                                        \
                if (num == HLT)                                                         \
                {                                                                       \
                    break;                                                              \
                }                                                                       \
                                                                                        \
                continue;                                                               \
            }

        #include "commands.def"
        #undef DEF_CMD
    }

    #undef arg_inm

    #undef arg_reg

    #undef arg_reg_inm

    #undef arg_jump

    printf ("\n\n label %s has ip %d", label[0].name, label[0].ip_save);

    fwrite (code_rb, sizeof (char), ip, output_file);

    if (ferror(output_file))
    {
        printf ("ERROR in function : %s \n"
                "writing file falled\n", __func__);
        return -1;
    }
    fclose (output_file);

    return 0;
}