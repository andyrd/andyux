global writeGdt

writeGdt:
        mov     eax, [esp+4]    ;get the first argument, the pointer to the gdt descriptor
        lgdt    [eax]           ;load the gdt descriptor

        jmp     0x08:.ldsr      ;reload the code segment registers, the only way to do this
                                ;is with a "far" jmp where we specify the cs value as the
                                ;segment in the jump address.  0x08 refers to the kernel 
                                ;code segment (each gdt entry is 8 bytes and it's the
                                ;second entry) 

.ldsr:
        mov     ax, 0x10        ;reload the data segment registers, 0x10 refers to the
        mov     ds, ax          ;kernel data segment (each gdt entry is 8 bytes and it's
        mov     es, ax          ;the third entry)
        mov     fs, ax
        mov     gs, ax
        mov     ss, ax

        ret