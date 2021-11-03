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

    FILE* input_file = fopen ("D:/VSC/processor/calc/code.rb", "rb");
    if (input_file == NULL)
    {
        printf ("ERROR in function : %s \n"
                "code.rb didn't open\n", __func__);
        return -1;
    }

    char* array_rb = make_array (input_file);
    if (array_rb == NULL)
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


    write_file (input_file, output_file, array_rb);

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
