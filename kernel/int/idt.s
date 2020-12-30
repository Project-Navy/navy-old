;; That one I did it myself :-)

[GLOBAL flush_idt]
flush_idt:
    sti
    lidt [rdi]
    ret