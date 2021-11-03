#include "asm.h"

static void arg_jump (Global* global, Bit_Type_Comand* bit_field, Labels* label, const char* lb_name);

static void arg      (Global* global, Bit_Type_Comand* bit_field,  const char reg, const elem_t value);


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

    Global global = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    global.code_rb = (char*) calloc (amount_operations, sizeof (elem_t) + 2 * sizeof (char));
    if (global.code_rb == NULL)
    {
        printf ("ERROR in function : %s \n" 
                "haven't memory for code_rb\n", __func__);
        return -1;
    }

    global.ip = 0;
    global.ammount_lbls = 0;
    global.not_need_reasm = 1;
    global.am_asm = 1;
    global.size_name = 0;
    global.amount_operations = amount_operations;


    Bit_Type_Comand bit_field = {0, 0, 0, 0};      


    for (global.cur_operation = 0; global.cur_operation < amount_operations; global.cur_operation++)
    {
        bit_field.ram = 0;
        bit_field.reg = 0;
        bit_field.inm = 0;

        int status_work = 0;

        global.line_operation = line[global.cur_operation].adress;
   
        printf("%s\n", line[global.cur_operation].adress);

        #define _parsing_default                        \
        parsing_default (&global, &bit_field, label)

        #define _parsing_push_pop                       \
        parsing_push_pop (&global, &bit_field, label)

        #define _parsing_call_jump                      \
        parsing_call_jump (&global, &bit_field, label)

        #define _parsing_                               \
        parsing (&global, &bit_field, label)

        #define _cmp_status_work_                                                       \
            if (status_work == 1)                                                       \
            {                                                                           \
                continue;                                                               \
            }                                                                           \
            if (status_work == -1)                                                      \
            {                                                                           \
                break;                                                                  \
            }   

        #define DEF_CMD(num, name, pars, ...)                                           \
            bit_field.cmd = num;                                                        \
            global.size_name = sizeof (#name);                                          \
            global.name_of_operaion = #name;                                            \
                                                                                        \
            status_work = _parsing_;                                                    \
            _cmp_status_work_                                                           \
            status_work = pars;                                                         \
            _cmp_status_work_

        #include "..\library\headers\commands.def"

        #undef DEF_CMD

        #undef _parsing_default
        #undef _parsing_push_pop
        #undef _parsing_call_jump
        #undef _parsing_

        printf ("BAD ASM");
        return -1;
    }

    for (int i = 0; i < AMMOUNT_LABELS; i++)
    {
        printf ("\n\n label %s has ip %d", label[i].name, label[i].ip_save);
    }

    if (fwrite (global.code_rb, sizeof (char), global.ip, output_file) == 0)
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

#define security_function       \
        cmp_zero_adr(global)    \
        cmp_zero_adr(bit_field) \
        cmp_zero_adr(label)

int parsing_default (Global* global, Bit_Type_Comand* bit_field,  Labels* label)
{   
    security_function

    int size_str = global->size_name - 1;

    if (strncmp (global->name_of_operaion, global->line_operation, size_str) == 0)
    {
        *((Bit_Type_Comand*)(global->code_rb + global->ip)) = *bit_field;
        global->ip = global->ip + 1;
        return 1;
    }
    return 0;
}

int parsing_call_jump (Global* global, Bit_Type_Comand* bit_field,  Labels* label)
{   
    security_function

    cmp_zero_adr(global)
    cmp_zero_adr(bit_field)
    cmp_zero_adr(label)

    char lb_name [MAX_AMOUNT_SMB] = {0};
    int size_str = global->size_name - 1;

    if (sscanf (global->line_operation, "CALL %s", lb_name) == 1)                  
    {                                                                       
        printf ("CALL %s\n", lb_name);                                      
        bit_field->cmd = CALL;                                               
        arg_jump (global, bit_field, label, lb_name);                                                    
        return 1;                                                         
    }

    if  ((*(global->line_operation) == 'J') &&
         (*(global->name_of_operaion) == 'J'))                                               
        {
            if ((strncmp (global->line_operation, global->name_of_operaion, size_str) == 0) &&                
                (sscanf (global->line_operation + global->size_name, "%s", lb_name) == 1))          
            {                                                                     
                arg_jump (global, bit_field, label, lb_name);  
                return 1;                                             
            }                                                                       
        }
    return 0;  
}

int parsing_push_pop (Global* global, Bit_Type_Comand* bit_field,  Labels* label)
{
    security_function

    int size_str = global->size_name - 1;

    elem_t value = 0;
    char reg = 0;

    char* pointer_on_str = global->line_operation + global->size_name; 

    if (((strncmp ("PUSH", global->name_of_operaion, size_str) == 0) ||                   
         (strncmp ("POP", global->name_of_operaion, size_str) == 0)) &&                   
         (strncmp (global->name_of_operaion, global->line_operation, size_str) == 0))                      
    {
        if ((*(global->line_operation + global->size_name) == '[') && 
            (*(global->line_operation + strlen (global->line_operation) - 1) == ']')) 
        {
            bit_field->ram = 1;
            /*add_pointer = 1;*/
            pointer_on_str = pointer_on_str + 1;
        }
        if ((sscanf (pointer_on_str,
                    "%cX + %lf", &reg, &value) == 2) &&
                    (isalpha (reg)))
        {
            bit_field->reg = 1;
            bit_field->inm = 1;      
            arg (global, bit_field, reg, value);
            return 1;                                               
        }
        if ((sscanf (pointer_on_str,                                        
                    "%cX", &reg) == 1) &&                                   
                    (isalpha (reg)))                                        
        {
            bit_field->reg = 1;
            bit_field->inm = 0; 
            arg (global, bit_field, reg, value);
            return 1;                                                   
        }                                                                   
        if (sscanf (pointer_on_str,                                         
                    "%lf", &value) == 1)                                  
        {                                                                
            bit_field->reg = 0;
            bit_field->inm = 1;                                     
            arg (global, bit_field, reg, value);
            return 1;                                 
        }                                                                  
    }
    return 0; 
}

int parsing (Global* global, Bit_Type_Comand* bit_field,  Labels* label)
{
    security_function

    char lb_name [MAX_AMOUNT_SMB] = {0};

    if (sscanf (global->line_operation, "$ %s :", lb_name) == 1)
    {
        printf ("$ %s :\n", lb_name);
        for (int am = 0; am < global->ammount_lbls; am++)
        {
            if (strncmp (label[am].name, lb_name,  
                        strlen (lb_name)) == 0)
            {
                label[am].ip_save =  global->ip;
                return 1;
            }
        }
        
        global->not_need_reasm = 0;

        label[global->ammount_lbls].name    =   global->line_operation + 2;
        label[global->ammount_lbls].ip_save =   global->ip;

        global->ammount_lbls = global->ammount_lbls + 1;

        return 1;                                                         
    }

    if ((global->am_asm < 2)                                     &&
        (global->not_need_reasm == false)                        && 
        (global->cur_operation == global->amount_operations - 1))
    {
        global->ip = 0;
        global->not_need_reasm = 1;
        global->cur_operation = -1;
        global->am_asm = global->am_asm + 1;
        return 1; 
    }
    return 0;                                                                          
}

static void arg (Global* global, Bit_Type_Comand* bit_field, const char reg, const elem_t value)
{
    *((Bit_Type_Comand*)(global->code_rb + global->ip)) = *bit_field;   
    global->ip = global->ip + 1;                                        
    if (bit_field->reg == 1)                                           
    {
        *(global->code_rb + global->ip) = reg - 'A';
        global->ip = global->ip + 1;           
    }
    if (bit_field->inm == 1)
    {
        *((elem_t*)(global->code_rb + global->ip)) = value;             
        printf ("%lf", value);                                          
        global->ip = global->ip + sizeof (elem_t);
    }
    return;
}

static void arg_jump (Global* global, Bit_Type_Comand* bit_field, Labels* label, const char* lb_name)
{
    bit_field->inm = 1;
    *((Bit_Type_Comand*)(global->code_rb + global->ip)) = *bit_field;
    global->ip = global->ip + 1;

    for (int am = 0; am < global->ammount_lbls; am++)
    {
        if (strncmp (label[am].name, lb_name,
                    strlen (lb_name)) == 0)
        {
            printf ("in %s write it %d with name %s\n",                         
                    lb_name, label[am].ip_save,                                 
                    label[am].name);                                            
            *((elem_t*)(global->code_rb + global->ip)) = label[am].ip_save;     
            break;                                                              
        }                                                                       
        printf ("in %s dont write it %d with name %s \n",                       
                lb_name, label[am].ip_save,                                     
                label[am].name);                                                
        global->not_need_reasm = 0;                                             
    }
    global->ip = global->ip + sizeof (elem_t);     
    return;     
}                               


#undef security_function

#undef cmp_zero_adr