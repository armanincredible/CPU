#include "asm.h"

#define cmp_zero_adr(name)                                  \
    if (name == NULL)                                       \
    {                                                       \
        printf ("\nERROR in function : %s \n"               \
                "%s have NULL ADRESS\n", #name, __func__);  \
        return -1;                                          \
    }

char *copy_in_buffer_from_file_asm (int* amount_operations)
{
    if (amount_operations == NULL)
    { 
        printf ("\nERROR in function : %s \n"
                "amount_operations have NULL ADRESS\n", __func__);
        return NULL;
    }

    char* buffer = NULL; 
   
    FILE* asm_file = fopen ("../library/txt_library/asm.txt", "r");
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
    cmp_zero_adr (line)

    cmp_zero_adr (label)

    FILE* output_file = fopen ("D:/VSC/processor/calc/code.rb", "wb");
    
    if (output_file == NULL)
    {
        printf ("ERROR in function : %s \n"
                "code.rb didn't opened\n", __func__);
        return -1;
    }

    char lb_name [MAX_AMOUNT_SMB] = {0};

    char * code_rb = (char*) calloc (amount_operations, sizeof (elem_t) + 2 * sizeof (char));
    if (code_rb == NULL)
    {
        printf ("ERROR in function : %s \n" 
                "haven't memory for code_rb\n", __func__);
        return -1;
    }

    int ip = 0;
    int ammount_lbls = 0;
    bool not_need_reasm = 1;
    int am_asm = 1;

    /*double val_dbl = 0;*/

    Bit_Type_Comand bit_field = {0, 0, 0, 0};      

    for (int cur_operation = 0; cur_operation < amount_operations; cur_operation++)
    {
        bit_field.ram = 0;
        bit_field.reg = 0;
        bit_field.inm = 0;
        int size_name_ = 0;
        int status_work = 0;

        char* line_operation = line[cur_operation].adress;
   
        /*elem_t value = 0;
        char reg = 0;*/
   
        printf("%s\n", line[cur_operation].adress);

        #define DEF_CMD(num, name, ...)                                                 \
            bit_field.cmd = num;                                                        \
            size_name_ = sizeof (#name);                                                \
                                                                                        \
            status_work = parsing (#name, line_operation, lb_name,                      \
                         &bit_field, code_rb, &ip,                                      \
                         &ammount_lbls, label, amount_operations, &cur_operation,       \
                         &not_need_reasm, &am_asm, size_name_);                         \
                                                                                        \
            if (status_work == 1)                                                       \
            {                                                                           \
                continue;                                                               \
            }                                                                           \
            if (status_work == -1)                                                      \
            {                                                                           \
                break;                                                                  \
            }                                                                           \
                                                                                        \
            if (strncmp (#name, line_operation, sizeof(#name) - 1) == 0)                \
            {                                                                           \
                *((Bit_Type_Comand*)(code_rb + ip)) = bit_field;                        \
                ip ++;                                                                  \
                continue;                                                               \
            }

        #include "..\library\headers\commands.def"

        #undef DEF_CMD

        printf ("BAD ASM");
        return -1;
    }

    for (int i = 0; i < AMMOUNT_LABELS; i++)
    {
        printf ("\n\n label %s has ip %d", label[i].name, label[i].ip_save);
    }

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

#define arg_inm(void)                                        \
        printf ("arg_inm %lf\n", value);                     \
        bit_field->reg = 0;                                  \
        bit_field->inm = 1;                                  \
        *((Bit_Type_Comand*)(code_rb + *ip)) = *bit_field;   \
        *ip = *ip + 1;                                       \
                                                             \
        *((elem_t*)(code_rb + *ip)) = value;                 \
        *ip = *ip + sizeof (elem_t);                              

#define arg_reg(void)                                        \
        printf ("arg_reg %cX\n", reg);                       \
        bit_field->reg = 1;                                  \
        bit_field->inm = 0;                                  \
        *((Bit_Type_Comand*)(code_rb + *ip)) = *bit_field;   \
        *ip = *ip + 1;                                       \
                                                             \
        *(code_rb + *ip) = reg - 'A';                        \
        *ip = *ip + 1;                                               


#define arg_reg_inm(void)                                    \
        printf ("arg_reg_inm %cX + %lf\n", reg, value);      \
        bit_field->reg = 1;                                  \
        bit_field->inm = 1;                                  \
        *((Bit_Type_Comand*)(code_rb + *ip)) = *bit_field;   \
        *ip = *ip + 1;                                       \
                                                             \
        *(code_rb + *ip) = reg - 'A';                        \
        *ip = *ip + 1;                                       \
                                                             \
        *((elem_t*)(code_rb + *ip)) = value;                 \
        *ip = *ip + sizeof (elem_t);                             

#define arg_jump(void)                                          \
        bit_field->inm = 1;                                     \
        *((Bit_Type_Comand*)(code_rb + *ip)) = *bit_field;      \
        *ip = *ip + 1;                                          \
                                                                \
        if (ammount_lbls == 0)                                  \
        {                                                       \
            not_need_reasm = 0;                                 \
        }                                                       \
        for (int am = 0; am < *ammount_lbls; am++)              \
        {                                                       \
            if (strncmp (label[am].name, lb_name,               \
                            label[am].str_len) == 0)            \
            {                                                   \
                printf ("in %s write it %d with name %s\n",     \
                        lb_name, label[am].ip_save,             \
                        label[am].name);                        \
                *((elem_t*)(code_rb + *ip)) = label[am].ip_save;\
                break;                                          \
            }                                                   \
            printf ("in %s dont write it %d with name %s \n",   \
                    lb_name, label[am].ip_save,                 \
                    label[am].name);                            \
            not_need_reasm = 0;                                 \
        }                                                       \
        *ip = *ip + sizeof (elem_t);                                         


int parsing (const char* name_of_operaion, char* line_operation, char* lb_name, Bit_Type_Comand* bit_field, char * code_rb, int* ip,
             int* ammount_lbls, Labels* label, const int amount_operations, int* cur_operation, bool* not_need_reasm, int* am_asm, const int size_name)
{
    cmp_zero_adr (name_of_operaion)

    cmp_zero_adr (line_operation)

    cmp_zero_adr (lb_name)

    cmp_zero_adr (bit_field)

    cmp_zero_adr (code_rb)

    cmp_zero_adr (ip)

    cmp_zero_adr (ammount_lbls)

    cmp_zero_adr (label)

    cmp_zero_adr (cur_operation)

    cmp_zero_adr (not_need_reasm)

    cmp_zero_adr (am_asm)

    elem_t value = 0;
    char reg = 0;
    int add_pointer = 0;
    double val_dbl = 0;

    /*int size_name = sizeof (name_of_operaion);*/
    int size_str = size_name - 1;

    char* pointer_on_str = line_operation + size_name + add_pointer;  

    if ((*not_need_reasm == false) && (*cur_operation == amount_operations - 1) && (*am_asm < 2))
    {
        printf ("\n\n\n\nhere");
        *ip = 0;
        *not_need_reasm = 1;
        *cur_operation = -1;
        *am_asm = *am_asm + 1;
        return 1; 
    }

    if (sscanf (line_operation, "$ %s :", lb_name) == 1)                    
    {                                                                       
        bool exit = 0;                                                      
        printf ("$ %s :\n", lb_name);                                           
        for (int am = 0; am < *ammount_lbls; am++)                               
        {                                                                       
            if (strncmp (label[am].name, lb_name,                               
                        label[am].str_len ) == 0)                               
            {
                label[am].ip_save =  *ip;                                       
                exit = 1;                                                       
                break;                                                          
            }                                                                   
        }                                                                       
        if (exit == 1){return 1;}
        *not_need_reasm = 0;
        label[*ammount_lbls].name    =   line_operation + 2;                
        label[*ammount_lbls].str_len =   strlen (lb_name) - 1;               
        label[*ammount_lbls].ip_save =   *ip;                            
        *ammount_lbls = *ammount_lbls + 1;                             
        return 1;                                                         
    }

    if (sscanf (line_operation, "CALL %s", lb_name) == 1)                  
    {                                                                       
        printf ("CALL %s\n", lb_name);                                      
        bit_field->cmd = CALL;                                               
        arg_jump(void)                                                      
        return 1;                                                         
    }

    if  ((*line_operation == 'J') &&
         (*name_of_operaion == 'J'))                                               
        {
            if ((strncmp (line_operation, name_of_operaion, size_str) == 0) &&                
                (sscanf (line_operation + size_name, "%s", lb_name) == 1))          
            {                                                                     
                arg_jump(void)   
                return 1;                                             
            }                                                                       
        }                                                                          
                                                                                    
    if (((strncmp ("PUSH", name_of_operaion, size_str) == 0) ||                   
         (strncmp ("POP", name_of_operaion, size_str) == 0)) &&                   
         (strncmp (name_of_operaion, line_operation, size_str) == 0))                      
    {
        if ((*(line_operation + size_name) == '[') && 
            (*(line_operation + strlen (line_operation) - 1) == ']')) 
        {
            bit_field->ram = 1;
            add_pointer = 1;
        }
        if ((sscanf (pointer_on_str,
                    "%cX + %lf", &reg, &val_dbl) == 2) &&
                    (isalpha (reg)))
        {
            value = val_dbl * FACTOR;
            arg_reg_inm(void)
            return 1;                                               
        }
        if ((sscanf (pointer_on_str,                                        
                    "%cX", &reg) == 1) &&                                   
                    (isalpha (reg)))                                        
        {                                                                   
            arg_reg(void)
            return 1;                                                   
        }                                                                   
        if (sscanf (pointer_on_str,                                         
                    "%lf", &val_dbl) == 1)                                  
        {                                                                   
            value = val_dbl * FACTOR;                                       
            arg_inm(void)
            return 1;                                                   
        }                                                                  
    }
    return 0;                                                                          
}

#undef arg_inm

#undef arg_reg

#undef arg_reg_inm

#undef arg_jump

#undef cmp_zero_adr