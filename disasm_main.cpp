#include "disasm.h"

int main (void)
{
    FILE* output_file = fopen ("disasm.txt", "w");
    if (output_file == NULL)
    {
        printf ("ERROR in function : %s \n"
                "disasm.txt didn't open\n", __func__);
        return -1;
    }

    FILE* input_file = fopen ("code.rb", "rb");
    if (input_file == NULL)
    {
        printf ("ERROR in function : %s \n"
                "code.rb didn't open\n", __func__);
        return -1;
    }

    elem_t* array = make_array (input_file);
    if (array == NULL)
    {
        printf ("ERROR in function : %s \n"
                "array have zero adress\n", __func__);
        return -1;
    }

    // #define PRINT_ERR(msg, err) PrintMyErrs (msg, err, __LINE__, __func__, __FILE__)

    // #define FUNCTION_ASSERT(condition, toDo, errToReturn)           do {
    //                                                                     if (condition) {
    //                                                                         toDo 
    //                                                                         return errToReturn;
    //                                                                     }
    //                                                                 } while (0)

    // FUNCTION_ASSERT (array == NULL, {PRINT_ERR ("nullptr on array");}, NULL_PTR_ON_ARRAY);

    // -E

    int amount = 0;
    int operation = 0;

    #define DEF_REGS(void)              \
            switch (value)              \
            {                           \
                case AX:               \
                {                       \
                    fprintf (output_file, "AX\n");     \
                    break;     \
                }                       \
                                        \
                case BX :               \
                {                       \
                    fprintf (output_file, "BX\n");         \
                    break;     \
                }                       \
                                            \
                case CX :               \
                {                       \
                    fprintf (output_file, "CX\n");     \
                    break;     \
                }                       \
                                                \
                case DX :                               \
                {                                           \
                    fprintf (output_file, "DX\n");\
                    break;     \
                }                                       \
            }
            
    #define DEF_CMD(num, name, ...)                                     \
            {                                                           \
                case name :                                             \
                {                                                       \
                    if (num == PUSH)                                    \
                    {                                                   \
                        elem_t value = array [amount + 1];              \
                        fprintf (output_file, "%s %d\n", #name, value); \
                        amount += 2;                                    \
                        break;                                          \
                    }                                                   \
                    if (num == POPARG)                                  \
                    {                                                   \
                        elem_t value = array [amount + 1];              \
                        fprintf (output_file, "POP ");                  \
                        DEF_REGS(void)                                  \
                        amount += 2;                                    \
                        break;                                          \
                    }                                                   \
                    fprintf (output_file, "%s\n", #name);               \
                    amount++;                                           \
                    break;                                              \
                }                                                       \
            }
    while (1)
    {   
        operation = array [amount];

        switch (operation)
        {
            #include "commands.def"
        }

        if (operation == HLT)
        {
            break;
        }
    }

    #undef DEF_CMD
    #undef DEF_REGS

    if (ferror(input_file))
    {
        printf ("ERROR in function : %s \n"
                "reading input_file falled\n", __func__);
        return -1;
    }
    
    if (ferror(output_file))
    {
        printf ("ERROR in function : %s \n"
                "writing output_file falled\n", __func__);
        return -1;
    }

    fclose (output_file);
    fclose (input_file);

    return 0;
}
