# CPU
This is my virtual processor using my assembler and [stack](https://github.com/armanincredible/stack_new).

## Comands
All processor instructions are wrapped in defines, so it's easy to add them. Here are some examples:
```
DEF_CMD(4, SUB, _parsing_default,
        elem_t fr_var = 0;
        elem_t sec_var = 0;
        stack_pop (stack, &fr_var);
        stack_pop (stack, &sec_var);

        stack_push (stack, fr_var - sec_var);
        )

DEF_CMD(5, MUL, _parsing_default,
        elem_t fr_var = 0;
        elem_t sec_var = 0;
        stack_pop (stack, &fr_var);
        stack_pop (stack, &sec_var);

        stack_push (stack, (elem_t) (fr_var * sec_var));
        )```
        
## Assembler
My assembler has registers, and a stack, as well as virtual memory.  <br/> 
Factorial example:
```
IN
POP AX
PUSH 1
POP BX
PUSH 1
POP CX
CALL FACTORIAL
PUSH CX
OUT
HLT
$ FACTORIAL :
        PUSH AX
        PUSH BX
        JAL END
        PUSH CX
        PUSH BX
        PUSH 1
        ADD
        MUL
        POP CX
        PUSH BX
        PUSH 1
        ADD
        POP BX
        CALL FACTORIAL
$ END :
        RET
```
