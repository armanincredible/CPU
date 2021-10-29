#include "disasm.h"

elem_t* make_array (FILE* input_file)
{
    if (input_file == NULL)
    {
        printf ("ERROR in function : %s \n"
                "input_file have NULL adress\n", __func__);
        return NULL;
    }

    int size = size_file (input_file);////////////////////////////////SIZE_TTTTTTTTTT

    elem_t* array = (elem_t*) calloc (size, sizeof(int));
    if (array == NULL)
    {
        printf ("ERROR in function : %s \n"
                "haven't memory for array\n", __func__);
        return NULL;
    }

    fread (array, sizeof (char), size, input_file);

    return array;
}