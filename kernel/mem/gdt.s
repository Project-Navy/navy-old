;; Took it from skiftOS
;; I don't code in assembly
;; Sorry not sorry
;; https://github.com/skiftOS/skift/blob/main/architectures/x86_64/kernel/helper.s

[ GLOBAL flush_gdt ]
flush_gdt:
    lgdt [rdi]
    push rbp
    mov rbp, rsp

    push qword 0x10
    push rbp
    pushf
    push qword 0x8
    push .trampoline
    iretq

.trampoline:
    pop rbp

    mov ax, 0x10

    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov ax, 0x28
    ltr ax

    ret
 