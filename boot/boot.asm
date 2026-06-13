bits 32
section .text
    align 4
    dd 0x1BADB002
    dd 0x00
    dd - (0x1BADB002 + 0x00)

global _start
extern kmain

_start:
    cli
    lgdt [gdt_descriptor]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush
flush:
    mov esp, stack_top
    push ebx
    push eax
    call kmain
    hlt

section .note.GNU-stack noalloc noexec nowrite

section .data
gdt_start:
    dd 0, 0
    dw 0xFFFF, 0x0000
    db 0x00, 0x9A, 0xCF, 0x00
    dw 0xFFFF, 0x0000
    db 0x00, 0x92, 0xCF, 0x00
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

section .bss
stack_bottom:
    resb 8192
stack_top:
