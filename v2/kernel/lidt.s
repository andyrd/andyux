global writeIdt

writeIdt:
        mov eax, [esp+4]        ;get the first argument, the pointer to the idt descriptor
        lidt [eax]              ;load the ldt descriptor
        ret