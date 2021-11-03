#include "asm.h"

int main (void) 
{
    int amount_operations = 0; 

    char* buffer = copy_in_buffer_from_file_asm (&amount_operations); 

    Onegin* line = (Onegin*) calloc (amount_operations, sizeof (Onegin));
    if (line == NULL)
    {
        printf ("ERROR in function : %s \n" 
                "haven't memory for struct line\n", __func__);
        return -1;
    }

    Labels* label = (Labels*) calloc (AMMOUNT_LABELS, sizeof (Labels));
    if (label == NULL)
    {
        printf ("ERROR in function : %s \n" 
                "haven't memory for struct line\n", __func__);
        return -1;
    }

    make_array_adress (buffer, amount_operations, line);

    make_code_rb (line, label, amount_operations);

    free (buffer);

    free (label);

    return 0;
}
