[GLOBAL idt_flush]

idt_flush:
    mov eax, [esp+4]  ; указатель на idt_ptr
    lidt [eax]        ; загружаем IDT в проц
    ret

[EXTERN current_task_esp]
[EXTERN irq_handler]

irq_common_stub:
    pusha             ; сохраняем все регистры
    mov ax, ds
    push eax

    mov ax, 0x10      ; сегмент ядра
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; --- многозадачность ---
    mov eax, esp
    push eax
    call irq_handler  ; уходим в планировщик
    mov esp, eax      ; подменяем стек — теперь другая задача
    ; -----------------------

    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8        ; чистим стек
    iret              ; возврат в задачу

[GLOBAL irq0]
[GLOBAL irq1]

; макрос IRQ
%macro IRQ 2
  [GLOBAL irq%1]
  irq%1:
    cli
    push byte 0       ; заглушка кода ошибки
    push byte %2      ; номер прерывания
    jmp irq_common_stub
%endmacro

IRQ 0, 32    ; таймер
IRQ 1, 33    ; клавиатура

[GLOBAL isr0]
isr0:
    cli
    push byte 0
    push byte 0
    jmp irq_common_stub
