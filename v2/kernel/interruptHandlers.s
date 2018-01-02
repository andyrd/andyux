extern cpuIsrRouter
extern irqIsrRouter        

%macro CPU_ISR_ERROR_CODE 1     ;interrupts 8, 10-14 inclusive push error codes onto the stack
global cpuIsr%1
cpuIsr%1:
        cli
        push %1                 ;interrupt number
        jmp cpuIsrCommonHandler
%endmacro
        
%macro CPU_ISR_NO_ERROR_CODE 1
global cpuIsr%1
cpuIsr%1:
        cli
        push byte 0             ;dummy error code
        push %1                 ;interrupt number
        jmp cpuIsrCommonHandler
%endmacro

%macro IRQ_ISR 2
        global irqIsr%1
irqIsr%1:
        cli
        push byte 0             ;dummy error code
        push byte %2            ;interrupt number
        jmp irqIsrCommonHandler
%endmacro        

CPU_ISR_NO_ERROR_CODE 0
CPU_ISR_NO_ERROR_CODE 1
CPU_ISR_NO_ERROR_CODE 2
CPU_ISR_NO_ERROR_CODE 3
CPU_ISR_NO_ERROR_CODE 4
CPU_ISR_NO_ERROR_CODE 5
CPU_ISR_NO_ERROR_CODE 6
CPU_ISR_NO_ERROR_CODE 7
CPU_ISR_ERROR_CODE    8
CPU_ISR_NO_ERROR_CODE 9
CPU_ISR_ERROR_CODE    10
CPU_ISR_ERROR_CODE    11
CPU_ISR_ERROR_CODE    12
CPU_ISR_ERROR_CODE    13
CPU_ISR_ERROR_CODE    14
CPU_ISR_NO_ERROR_CODE 15
CPU_ISR_NO_ERROR_CODE 16
CPU_ISR_NO_ERROR_CODE 17
CPU_ISR_NO_ERROR_CODE 18

IRQ_ISR  0, 32
IRQ_ISR  1, 33
IRQ_ISR  2, 34
IRQ_ISR  3, 35
IRQ_ISR  4, 36
IRQ_ISR  5, 37
IRQ_ISR  6, 38
IRQ_ISR  7, 39
IRQ_ISR  8, 40
IRQ_ISR  9, 41
IRQ_ISR 10, 42
IRQ_ISR 11, 43
IRQ_ISR 12, 44
IRQ_ISR 13, 45
IRQ_ISR 14, 46
IRQ_ISR 15, 47        
        
cpuIsrCommonHandler:
        pusha

        mov ax, ds
        push eax

        mov ax, 0x10
        mov ds, ax
        mov es, ax   
        mov fs, ax
        mov gs, ax

        call cpuIsrRouter

        pop eax

        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax

        popa
        add esp, 8              ;cleans up the pushed error code and and ISR number

        sti
        iret

irqIsrCommonHandler:
        pusha

        mov ax, ds
        push eax

        mov ax, 0x10
        mov ds, ax
        mov es, ax   
        mov fs, ax
        mov gs, ax

        call irqIsrRouter

        pop ebx

        mov ds, bx
        mov es, bx
        mov fs, bx
        mov gs, bx

        popa
        add esp, 8              ;cleans up the pushed error code and and ISR number

        sti
        iret
        