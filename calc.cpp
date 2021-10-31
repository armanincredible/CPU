#include "calc.h"

#define proc_dump_write(void)                                                               \
        fprintf (calc_dump, "ram_status = %u, reg_status = %u, inm_status %u, cmd = %d\n"   \
                            "IP %d, REG_VALUE %d, INM_VALUE %d\n\n\n",                      \
                            bit_field.ram, bit_field.reg, bit_field.inm, bit_field.cmd,     \
                            proc->Ip, reg_value, inm_value);


elem_t calculate (Stack* stack, Stack* rtr_stack, Proc_str* proc)
{
    if (stack == NULL)
    {
        printf ("ERROR in function : %s \n"
                "stack have zero adress\n", __func__);
        return -1;
    }

    if (proc == NULL)
    {
        printf ("ERROR in function : %s \n"
                "proc have zero adress\n", __func__);
        return -1;
    }


    FILE* input_file = fopen ("code.rb", "rb");
    if (input_file == NULL)
    {
        printf ("ERROR in function : %s \n"
                "code.rb didn't open\n", __func__);
        return -1;
    }

    FILE* calc_dump = fopen ("calc_status.txt", "w");
    if (calc_dump == NULL)
    {
        printf ("ERROR in function : %s \n"
                "calcstatus.txt didn't open\n", __func__);
        return -1;
    }
    

    proc->Code = make_array (input_file);
    if (proc->Code == NULL)
    {
        printf ("ERROR in function : %s \n"
                "array have zero adress\n", __func__);
        return -1;
    }

    proc->Ram = (elem_t*) calloc (SIZE_RAM, sizeof(elem_t));
    if (proc->Ram == NULL)
    {
        printf ("ERROR in function : %s \n"
                "array have zero adress\n", __func__);
        return -1;
    }
    memset (proc->Ram, 0, SIZE_RAM);

    int operation = 0;
    proc->Ip = 0;

    int reg_value = 0;
    int inm_value = 0;

    BITS bits = {0};
    Bit_Type_Comand bit_field = {0};

    while (1)
    {   
        bit_field = *((Bit_Type_Comand*)(proc->Code + proc->Ip));

        inm_value = 0;
        reg_value = 0;

        take_arguments (proc->Code, &(proc->Ip), &reg_value, &inm_value, &bit_field);
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
            #include "D:\VSC\processor\library\commands.def"
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

#undef proc_dump_write