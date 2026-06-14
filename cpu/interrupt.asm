[GLOBAL idt_flush]

idt_flush:
    mov eax, [esp+4]  ; pointer to idt_ptr
    lidt [eax]        ; load IDT into CPU
    ret

[EXTERN current_task_esp]
[EXTERN irq_handler]

irq_common_stub:
    pusha             ; save all registers
    mov ax, ds
    push eax

    mov ax, 0x10      ; kernel segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; --- multitasking ---
    mov eax, esp
    push eax
    call irq_handler  ; go to scheduler
    mov esp, eax      ; swap stack — now another task
    ; -------------------

    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8        ; clean stack
    iret              ; return to task

[GLOBAL irq0]
[GLOBAL irq1]

; IRQ macro
%macro IRQ 2
  [GLOBAL irq%1]
  irq%1:
    cli
    push byte 0       ; error code stub
    push byte %2      ; interrupt number
    jmp irq_common_stub
%endmacro

IRQ 0, 32    ; timer
IRQ 1, 33    ; keyboard

[GLOBAL isr0]
isr0:
    cli
    push byte 0
    push byte 0
    jmp irq_common_stub
