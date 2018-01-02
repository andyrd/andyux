BITS 16

start:        
        ;; set up segments
        
        mov ax, 0x07C0  
        mov ds, ax
        mov es, ax

        ;; welcome

        mov si, welcome
        call printString

        ;; main loop

mainLoop:
        mov si, prompt
        call printString

        mov di, inputBuffer
        call readString

        mov si, inputBuffer
        
        cmp byte [si], 0
        je mainLoop

        mov si, inputBuffer
        mov di, cmdHelp
        call compareStrings
        jc .help

        mov si, invalidCmdMsg
        call printString

        jmp mainLoop
        
.help:
        mov si, helpMsg
        call printString
        jmp mainLoop

        ;; data

        welcome db 0x0D, 0x0A, 'Welcome to Andyux v1.0', 0x0D, 0x0A, 0x0D, 0x0A, 0
        prompt db '>', 0
        cmdHelp db 'help', 0
        helpMsg db 'Never fear! Johnny is near!', 0x0D, 0x0A, 0
        invalidCmdMsg db 'Invalid command', 0x0D, 0x0A, 0
        inputBuffer times 0xEF db 0

        ;; functions
        
printString:		
        mov ah, 0x0E

.loop:
        lodsb
        cmp al, 0
        je .done
        int 0x10
        jmp .loop

.done:
        ret

readString:
        xor cl, cl
        
.loop:
        mov ah, 0
        int 0x16

        cmp al, 0x08
        je .backspace

        cmp al, 0x0D
        je .done

        cmp cl, 0xEF
        je .loop

        mov ah, 0x0E
        int 0x10      

        stosb        
        inc cl

        jmp .loop

.backspace:
        cmp cl, 0
        je .loop

        dec di
        mov byte [di], 0
        dec cl

        mov ah, 0x0E
        mov al, 0x08
        int 0x10

        mov al, ' '
        int 0x10

        mov al, 0x08
        int 0x10

        jmp .loop
        
.done:
        mov al, 0
        stosb

        mov ah, 0x0E
        mov al, 0x0D
        int 0x10

        mov al, 0x0A
        int 0x10
        
        ret

        compareStrings:
.loop:        
        mov al, [si]
        mov bl, [di]

        cmp al, bl
        jne .notEqual

        cmp al, 0
        je .equal

        inc si
        inc di

        jmp .loop
        
.notEqual:
        clc
        ret

.equal:
        stc
        ret

        ;; fill remainder with zeros and sign with bootloader signature

        times 510-($-$$) db 0	
        dw 0xAA55
        