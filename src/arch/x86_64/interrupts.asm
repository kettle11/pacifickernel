
bits 64
section .text

; Loads the IDT defined in '_idtp' into the processor.
; This is declared in C as 'extern void idt_load();'
global _idt_load
extern idtp
_idt_load:
    lidt [idtp]
    ret

; In just a few pages in this tutorial, we will add our Interrupt
; Service Routines (ISRs) right here!
global _isr0
global _isr1
global _isr2
global _isr3
global _isr4
global _isr5
global _isr6
global _isr7
global _isr8
global _isr9
global _isr10
global _isr11
global _isr12
global _isr13
global _isr14
global _isr15
global _isr16
global _isr17
global _isr18
global _isr19
global _isr20
global _isr21
global _isr22
global _isr23
global _isr24
global _isr25
global _isr26
global _isr27
global _isr28
global _isr29
global _isr30
global _isr31

;  0: Divide By Zero Exception
_isr0:
    cli
    push qword 0
    push qword 0
    jmp isr_common_stub

;  1: Debug Exception
_isr1:
    cli
    push qword 0
    push qword 1
    jmp isr_common_stub

;  2: Non Maskable Interrupt Exception
_isr2:
    cli
    push qword 0
    push qword 2
    jmp isr_common_stub

;  3: Int 3 Exception
_isr3:
    cli
    push qword 0
    push qword 3
    jmp isr_common_stub

;  4: INTO Exception
_isr4:
    cli
    push qword 0
    push qword 4
    jmp isr_common_stub

;  5: Out of Bounds Exception
_isr5:
    cli
    push qword 0
    push qword 5
    jmp isr_common_stub

;  6: Invalid Opcode Exception
_isr6:
    cli
    push qword 0
    push qword 6
    jmp isr_common_stub

;  7: Coprocessor Not Available Exception
_isr7:
    cli
    push qword 0
    push qword 7
    jmp isr_common_stub

;  8: Double Fault Exception (With Error Code!)
_isr8:
    cli
    push qword 8
    jmp isr_common_stub

;  9: Coprocessor Segment Overrun Exception
_isr9:
    cli
    push qword 0
    push qword 9
    jmp isr_common_stub

; 10: Bad TSS Exception (With Error Code!)
_isr10:
    cli
    push qword 10
    jmp isr_common_stub

; 11: Segment Not Present Exception (With Error Code!)
_isr11:
    cli
    push qword 11
    jmp isr_common_stub

; 12: Stack Fault Exception (With Error Code!)
_isr12:
    cli
    push qword 12
    jmp isr_common_stub

; 13: General Protection Fault Exception (With Error Code!)
_isr13:
    cli
    push qword 13
    jmp isr_common_stub

; 14: Page Fault Exception (With Error Code!)
_isr14:
    cli
    push qword 14
    jmp isr_common_stub

; 15: Reserved Exception
_isr15:
    cli
    push qword 0
    push qword 15
    jmp isr_common_stub

; 16: Floating Point Exception
_isr16:
    cli
    push qword 0
    push qword 16
    jmp isr_common_stub

; 17: Alignment Check Exception
_isr17:
    cli
    push qword 0
    push qword 17
    jmp isr_common_stub

; 18: Machine Check Exception
_isr18:
    cli
    push qword 0
    push qword 18
    jmp isr_common_stub

; 19: Reserved
_isr19:
    cli
    push qword 0
    push qword 19
    jmp isr_common_stub

; 20: Reserved
_isr20:
    cli
    push qword 0
    push qword 20
    jmp isr_common_stub

; 21: Reserved
_isr21:
    cli
    push qword 0
    push qword 21
    jmp isr_common_stub

; 22: Reserved
_isr22:
    cli
    push qword 0
    push qword 22
    jmp isr_common_stub

; 23: Reserved
_isr23:
    cli
    push qword 0
    push qword 23
    jmp isr_common_stub

; 24: Reserved
_isr24:
    cli
    push qword 0
    push qword 24
    jmp isr_common_stub

; 25: Reserved
_isr25:
    cli
    push qword 0
    push qword 25
    jmp isr_common_stub

; 26: Reserved
_isr26:
    cli
    push qword 0
    push qword 26
    jmp isr_common_stub

; 27: Reserved
_isr27:
    cli
    push qword 0
    push qword 27
    jmp isr_common_stub

; 28: Reserved
_isr28:
    cli
    push qword 0
    push qword 28
    jmp isr_common_stub

; 29: Reserved
_isr29:
    cli
    push qword 0
    push qword 29
    jmp isr_common_stub

; 30: Reserved
_isr30:
    cli
    push qword 0
    push qword 30
    jmp isr_common_stub

; 31: Reserved
_isr31:
    cli
    push qword 0
    push qword 31
    jmp isr_common_stub

ret;
; We call a C function in here. We need to let the assembler know
; that '_fault_handler' exists in another file
extern _fault_handler

; This is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
isr_common_stub:
    
	; save general purpose registers
	push qword r15
	push qword r14
	push qword r13
	push qword r12
	push qword r11
	push qword r10
	push qword r9
	push qword r8
	push qword rax
	push qword rcx
	push qword rdx
	push qword rbx
	push qword rbp
	push qword rsi
	push qword rdi

	mov qword rdi,rsp     ; pass the  iframe using rdi 

	;extern x86_exception_handler
	;call x86_exception_handler

	extern fault_handler
	call fault_handler

	; restore general purpose registers */
	pop qword rdi
	pop qword rsi
	pop qword rbp
	pop qword rbx
	pop qword rdx
	pop qword rcx
	pop qword rax
	pop qword r8
	pop qword r9
	pop qword r10
	pop qword r11
	pop qword r12
	pop qword r13
	pop qword r14
	pop qword r15

	add rsp, 16
	iretq

global _irq0
global _irq1
global _irq2
global _irq3
global _irq4
global _irq5
global _irq6
global _irq7
global _irq8
global _irq9
global _irq10
global _irq11
global _irq12
global _irq13
global _irq14
global _irq15

; 32: IRQ0
_irq0:
    cli
    push qword 0
    push qword 32
    jmp irq_common_stub

; 33: IRQ1
_irq1:
    cli
    push qword 0
    push qword 33
    jmp irq_common_stub

; 34: IRQ2
_irq2:
    cli
    push qword 0
    push qword 34
    jmp irq_common_stub

; 35: IRQ3
_irq3:
    cli
    push qword 0
    push qword 35
    jmp irq_common_stub

; 36: IRQ4
_irq4:
    cli
    push qword 0
    push qword 36
    jmp irq_common_stub

; 37: IRQ5
_irq5:
    cli
    push qword 0
    push qword 37
    jmp irq_common_stub

; 38: IRQ6
_irq6:
    cli
    push qword 0
    push qword 38
    jmp irq_common_stub

; 39: IRQ7
_irq7:
    cli
    push qword 0
    push qword 39
    jmp irq_common_stub

; 40: IRQ8
_irq8:
    cli
    push qword 0
    push qword 40
    jmp irq_common_stub

; 41: IRQ9
_irq9:
    cli
    push qword 0
    push qword 41
    jmp irq_common_stub

; 42: IRQ10
_irq10:
    cli
    push qword 0
    push qword 42
    jmp irq_common_stub

; 43: IRQ11
_irq11:
    cli
    push qword 0
    push qword 43
    jmp irq_common_stub

; 44: IRQ12
_irq12:
    cli
    push qword 0
    push qword 44
    jmp irq_common_stub

; 45: IRQ13
_irq13:
    cli
    push qword 0
    push qword 45
    jmp irq_common_stub

; 46: IRQ14
_irq14:
    cli
    push qword 0
    push qword 46
    jmp irq_common_stub

; 47: IRQ15
_irq15:
    cli
    push qword 0
    push qword 47
    jmp irq_common_stub


extern _irq_handler

irq_common_stub:

	; save general purpose registers
	push qword r15
	push qword r14
	push qword r13
	push qword r12
	push qword r11
	push qword r10
	push qword r9
	push qword r8
	push qword rax
	push qword rcx
	push qword rdx
	push qword rbx
	push qword rbp
	push qword rsi
	push qword rdi

	mov qword rdi,rsp     ; pass the  iframe using rdi 

	;extern x86_exception_handler
	;call x86_exception_handler

	extern irq_handler
	call irq_handler

	; restore general purpose registers */
	pop qword rdi
	pop qword rsi
	pop qword rbp
	pop qword rbx
	pop qword rdx
	pop qword rcx
	pop qword rax
	pop qword r8
	pop qword r9
	pop qword r10
	pop qword r11
	pop qword r12
	pop qword r13
	pop qword r14
	pop qword r15

	add rsp, 16
	iretq