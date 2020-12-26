[ GLOBAL flush_gdt ]
flush_gdt:
    push rbp 
    mov rbp, rsp

    lgdt [rdi]


    mov ax, 0x10
    mov ds, ax 
    mov es, ax
    mov fs, ax 
    mov gs, ax 
    mov ss, ax 

    mov rax, qword .gdt_end
    push rsi 
    push rax 
    iretq

.gdt_end:
    pop rbp

    mov ax, 0x28
    ltr ax

    ret

    