.text
        .global _start

write:  mov         $1, %al
        syscall
        ret

_start: mov     $0x0a202020216f646e, %rax # "ndo!\n  "
        push    %rax
        mov     $0x756d202c616c6f68, %rax # "hola, mu"
        push    %rax
        xor     %rax, %rax
        mov     %rsp, %rsi
        mov     $1, %rdi
        mov     $16, %rdx
        call    write

exit:   xor     %rax, %rax
        mov     $60, %rax
        syscall

