        if (sscanf (line[i].adress, "PUSH [%cX + %d]", reg, value) == 2)
        {
            printf ("PUSH [%cX + %d]");
            bit_field.ram = 1;
            bit_field.reg = 1;
            bit_field.inm = 1;
            bit_field.cmd = PUSH;

            *((Bit_Type_Comand*)(code_rb + ip)) = bit_field; 
            ip ++;
            //////def_regs
            *((int*)(code_rb + ip)) = value;
            ip++;
        }
        if (sscanf (line[i].adress, "PUSH [%cX]", reg) == 1)
        {
            printf ("PUSH [%cX]");
            bit_field.ram = 1;
            bit_field.reg = 1;
            bit_field.inm = 0;
            bit_field.cmd = PUSH;

            *((Bit_Type_Comand*)(code_rb + ip)) = bit_field; 
            ip ++;
            //////def_regs
        }
        if (sscanf (line[i].adress, "PUSH [%d]", value) == 1)
        {
            printf ("PUSH [%d]");
            bit_field.ram = 1;
            bit_field.reg = 0;
            bit_field.inm = 1;
            bit_field.cmd = PUSH;

            *((Bit_Type_Comand*)(code_rb + ip)) = bit_field; 
            ip ++;
            *((int*)(code_rb + ip)) = value;
            ip++;
        }
        if (sscanf (line[i].adress, "PUSH %cX + %d", reg, value) == 2)
        {
            printf ("PUSH %cX + %d");
            bit_field.ram = 0;
            bit_field.reg = 1;
            bit_field.inm = 1;
            bit_field.cmd = PUSH;

            *((Bit_Type_Comand*)(code_rb + ip)) = bit_field; 
            ip ++;
            //////def_regs
            *((int*)(code_rb + ip)) = value;
            ip++;
        }
        if (sscanf (line[i].adress, "PUSH %cX", reg) == 1)
        {
            printf ("PUSH %cX");
            bit_field.ram = 0;
            bit_field.reg = 1;
            bit_field.inm = 0;
            bit_field.cmd = PUSH;

            *((Bit_Type_Comand*)(code_rb + ip)) = bit_field; 
            ip ++;
            //////def_regs
            *((int*)(code_rb + ip)) = value;
            ip++;
        }
        if (sscanf (line[i].adress, "PUSH %d", value) == 1)
        {
            printf ("PUSH %d");
            bit_field.ram = 0;
            bit_field.reg = 0;
            bit_field.inm = 1;
            bit_field.cmd = PUSH;

            *((Bit_Type_Comand*)(code_rb + ip)) = bit_field; 
            ip ++;
            //////def_regs
            *((int*)(code_rb + ip)) = value;
            ip++;
        }
        if (sscanf (line[i].adress, "POP [%cX + %d]", reg, value) == 2)
        {
            printf ("POP [%cX + %d]");
            bit_field.ram = 1;
            bit_field.reg = 1;
            bit_field.inm = 1;
            bit_field.cmd = POP;

            *((Bit_Type_Comand*)(code_rb + ip)) = bit_field; 
            ip ++;
            //////def_regs
            *((int*)(code_rb + ip)) = value;
            ip++;
        }

        if (sscanf (line[i].adress, "POP [%cX]", reg) == 1)
        {
            printf ("POP [%cX]");
            bit_field.ram = 1;
            bit_field.reg = 1;
            bit_field.inm = 0;
            bit_field.cmd = POP;

            *((Bit_Type_Comand*)(code_rb + ip)) = bit_field; 
            ip ++;
            //////def_regs
        }

        if (sscanf (line[i].adress, "POP [%d]", value) == 1)
        {
            printf ("POP [%d]");
            bit_field.ram = 1;
            bit_field.reg = 0;
            bit_field.inm = 1;
            bit_field.cmd = POP;

            *((Bit_Type_Comand*)(code_rb + ip)) = bit_field; 
            ip ++;
            *((int*)(code_rb + ip)) = value;
            ip++;
        }

        if (sscanf (line[i].adress, "POP %cX", reg, value) == 1)
        {
            printf ("POP %cX");
            bit_field.ram = 0;
            bit_field.reg = 0;
            bit_field.inm = 1;
            bit_field.cmd = POP;

            *((Bit_Type_Comand*)(code_rb + ip)) = bit_field; 
            ip ++;
            *((int*)(code_rb + ip)) = value;
            ip++;
        }