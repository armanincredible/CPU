#include "calc.h"

int main (void)
{
    Proc_str proc = {};
    proc.st = {};
    stack_ctor (&proc.st, BASIC_SIZE);
    
    proc.Regs = (elem_t*) calloc (amount_regs, sizeof (elem_t));
    if (proc.Regs == NULL)
    {
        printf ("ERROR in function : %s \n"
        "havent memory for regs\n", __func__);
        return -1;
    }
    memset (proc.Regs, 0, amount_regs);

    calculate (&proc.st, &proc);

    free (proc.Regs);

    stack_dump (&proc.st, __FILE__, __func__, __LINE__);

    stack_dtor (&proc.st);

    return 0;
}

/*int cmp_files (void)
{
    struct stat status_asm; 
    stat ("asm.txt", &status_asm); //TODO CMP -1 ARGV FILES

    struct stat status_code; 
    stat ("code.rb", &status_code);

    if ((status_code.st_size == 0) ||
        (status_asm.st_mtime > status_code.st_mtime))
    {
        return 1;
    }
    else 
    {
        return 0;
    }
}*/
