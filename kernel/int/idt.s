;; That one I did it myself :-)

[GLOBAL flush_idt]
flush_idt:
    lidt [rdi]
    ret