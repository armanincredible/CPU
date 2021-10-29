#ifndef CODEOPERATIONS
#define CODEOPERATIONS

#define DEF_CMD(num, name, ...) \
        name = num,

enum def_commands
{
    #include "commands.def"
};

struct Bit_Type_Comand
{
    unsigned ram : 1;
    unsigned reg : 1;
    unsigned inm : 1;

    unsigned cmd : 5;
};

//!TODO double
//double num;
//char byteTransf [] = (char*)(&num);

enum regs
{
    AX = 0,
    BX,
    CX,
    DX
};

#undef DEF_CMD

#endif


/*enum operations
{
    ERROR = -1,
    HLT,
    PUSH,
    POP,
    ADD,
    SUB,
    MUL,
    DIV,
    OUT
};*/