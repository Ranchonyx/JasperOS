; program.asm
[BITS 32]

pusha
cld
mov edi, 0x000b8000
mov cx, 2000
mov ah, 7
mov al, ' '
rep stosw

popa
ret
