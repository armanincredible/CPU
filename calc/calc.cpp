#include "calc.h"

#define proc_dump_write(void)                                                               \
        fprintf (calc_dump, "ram_status = %d, reg_status = %d, inm_status %d, cmd = %d\n"   \
                            "IP %d, REG_VALUE %d, INM_VALUE %lg\n\n\n",                     \
                            bit_field.ram, bit_field.reg, bit_field.inm, bit_field.cmd,     \
                            proc->Ip, reg_value, inm_value);

#define cmp_zero_adr(name)                                  \
    if (name == NULL)                                       \
    {                                                       \
        printf ("\nERROR in function : %s \n"               \
                "%s have NULL ADRESS\n", #name, __func__);  \
        return -1;                                          \
    }

elem_t calculate (Stack* stack, Proc_str* proc)
{
    cmp_zero_adr(stack)

    cmp_zero_adr(proc)


    FILE* input_file = fopen ("code.rb", "rb");
    cmp_zero_adr(input_file)

    FILE* calc_dump = fopen ("calc_status.txt", "w");
    cmp_zero_adr(calc_dump)
    
    int rtr_pointer [AMMOUNT_SAVES_IP] = {0};
    int rtr_ip = 0;

    proc->Code = make_array (input_file);
    cmp_zero_adr(proc->Code)

    proc->Ram = (elem_t*) calloc (SIZE_RAM, sizeof(elem_t));
    cmp_zero_adr(proc->Ram)

    memset (proc->Ram, 0, SIZE_RAM);

    int operation = 0;
    proc->Ip = 0;

    int reg_value = 0;
    elem_t inm_value = 0;

    BITS bits = {0, 0};
    Bit_Type_Comand bit_field = {0, 0, 0, 0};

    while (1)
    {   
        bit_field = *((Bit_Type_Comand*)(proc->Code + proc->Ip));

        inm_value = 0;
        reg_value = 0;

        if (take_arguments (proc->Code, &(proc->Ip), &reg_value, &inm_value, &bit_field) == -1)
        {
            break;
        }

        proc_dump_write(void) 

        operation = bit_field.cmd;

        if (operation == HLT)
        {
            break;
        }

        #define DEF_CMD(num, name, hz, arg)     \
                case name:                      \
                {                               \
                    arg                         \
                    break;                      \
                }                               

        switch (operation)
        {
            #include "..\library\headers\commands.def"
        }
        
        #undef DEF_CMD

    }

    /*printf ("%d", *(proc->Regs + 0));
    printf ("%d", *(proc->Regs + 1));
    printf ("%d\n", *(proc->Regs + 2));*/

    free(proc->Ram);
    free(proc->Code);
    
    if (ferror(input_file))
    {
        printf ("ERROR in function : %s \n"
                "reading input_file falled\n", __func__);
        return -1;
    }
    fclose (input_file);

    if (ferror(calc_dump))
    {
        printf ("ERROR in function : %s \n"
                "writing calc_dump falled\n", __func__);
        return -1;
    }
    fclose (calc_dump);

    return 0;
}

char* make_array (FILE* input_file)
{
    if (input_file == NULL)
    {
        printf ("ERROR in function : %s \n"
                "input_file have NULL adress\n", __func__);
        return NULL;
    }

    int size = size_file (input_file);

    char* array = (char*) calloc (size, sizeof(char));
    if (array == NULL)
    {
        printf ("ERROR in function : %s \n"
                "haven't memory for array\n", __func__);
        return NULL;
    }

    if (fread (array, sizeof (char), size, input_file) == 0)
    {
        printf ("ERROR in function : %s \n"
                "writing falled\n", __func__);
        return NULL;
    }

    return array;
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

#undef cmp_zero_adr

#undef proc_dump_write