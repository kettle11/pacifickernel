/* IDT (Interrupt Descriptor Table) */
/* lidt (load interrupt descriptor table) */

#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>

#define IDT_SIZE 256

#include "helper.c"
#include <system.h>

struct IDT_Entry {
   uint16_t offset_1; // offset bits 0..15
   uint16_t selector; // a code segment selector in GDT or LDT
   uint8_t ist;       // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
   uint8_t type_attr; // type and attributes
   uint16_t offset_2; // offset bits 16..31
   uint32_t offset_3; // offset bits 32..63
   uint32_t zero;     // reserved
}; __attribute__((packed));

struct x86_64_iframe {
    uint64_t di, si, bp, bx, dx, cx, ax;                // pushed by common handler
    uint64_t r8, r9, r10, r11, r12, r13, r14, r15;      // pushed by common handler
    uint64_t vector;                                    // pushed by stub
    uint64_t err_code;                                  // pushed by interrupt or stub
    uint64_t ip, cs, flags;                             // pushed by interrupt
    uint64_t user_sp, user_ss;                          // pushed by interrupt if priv change occurs
};

struct idt_ptr
{
    unsigned short limit;
    void* base;
} __attribute__((packed));

struct IDT_Entry idt[256];
struct idt_ptr idtp;

void (*interrupt_handlers[256-32])(void);

// Part of system.h
void register_handler(void(*interrupt_handler), int index)
{
	int actualIndex = index - 32;

	if (actualIndex >= 0 && actualIndex < 256-32)
	{
		interrupt_handlers[actualIndex] = interrupt_handler;
	}
}

// Part of system.h
void unregister_handler(int index)
{
	int actualIndex = index - 32;

	if (actualIndex >= 0 && actualIndex < 256-32)
	{
		interrupt_handlers[actualIndex] = 0;
	}
}

void setup_handlers()
{
	memset(&interrupt_handlers, 0, sizeof(interrupt_handlers));
}

void call_handler(int index)
{
	int actualIndex = index - 32;

	if (actualIndex >= 0 && actualIndex < 256-32)
	{
		if (interrupt_handlers[actualIndex] != 0)
		{

			interrupt_handlers[actualIndex]();
		}
	}
}

extern void _idt_load();
extern void terminal_writestring(const char* data); // This shouldn't be here

/* Use this function to set an entry in the IDT. Alot simpler
*  than twiddling with the GDT ;) */
void idt_set_gate(unsigned char num, void *base, uint16_t sel, uint8_t flags)
{
	uint64_t baseInt = (uint64_t)base;
    /* The interrupt routine's base address */
    idt[num].offset_1 = (baseInt & 0xFFFF);

    /* The segment or 'selector' that this IDT entry will use
    *  is set here, along with any access flags */
    idt[num].selector = sel;
    idt[num].ist = 0; // I don't know what this is

    idt[num].type_attr = flags;
    idt[num].offset_2 = (baseInt >> 16) & 0xFFFF;
	idt[num].offset_3 = (baseInt >> 32) & 0xFFFFFFFF;

    idt[num].zero = 0;
}


/* Installs the IDT */
void idt_install()
{
    /* Sets the special IDT pointer up, just like in 'gdt.c' */
    idtp.limit = (sizeof (struct IDT_Entry) * 256) - 1;
    idtp.base = &idt;

    /* Clear out the entire IDT, initializing it to zeros */
    memset(&idt, 0, sizeof(struct IDT_Entry) * 256);

    /* Add any new ISRs to the IDT here using idt_set_gate */

    /* Points the processor's internal register to the new IDT */
    _idt_load();
}


/* All of our Exception handling Interrupt Service Routines will
*  point to this function. This will tell us what exception has
*  happened! Right now, we simply halt the system by hitting an
*  endless loop. All ISRs disable interrupts while they are being
*  serviced as a 'locking' mechanism to prevent an IRQ from
*  happening and messing up kernel data structures */
void fault_handler(struct x86_64_iframe *frame)
{
	terminal_writestring(" Exception. System Halted! Exception Number: ");
        terminal_writeint(frame->vector);
        terminal_writestring("\n");

     terminal_writestring("Error code: ");
        terminal_writeint(frame->err_code);
        terminal_writestring("\n");
           
    terminal_writestring("Instruction Pointer: ");
        terminal_writeintbase(frame->ip, 16);
        terminal_writestring("\n");
              
    /* Is this a fault whose number is from 0 to 31? */
    if (frame->vector > 32)
    {
        /* Display the description for the Exception that occurred.
        *  In this tutorial, we will simply halt the system using an
        *  infinite loop */
       // puts(exception_messages[r->int_no]);
        terminal_writestring(" Exception greater than 32 ");
    }

	for (;;);
}
	

void irq_handler(struct x86_64_iframe *frame)
{
	// Pass the call of to a registered handler (presently there can only be one)
	call_handler(frame->vector);

	/*
    terminal_writestring("IRQ request: ");
        terminal_writeint(frame->vector);
        terminal_writestring("\n");*/
           
    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 15), then we need to send an EOI to
    *  the slave controller */
    if (frame->vector >= 40)
    {
        outp(0xA0, 0x20);
    }

    /* In either case, we need to send an EOI to the master
    *  interrupt controller too */
    outp(0x20, 0x20);
}


/* These are function prototypes for all of the exception
*  handlers: The first 32 entries in the IDT are reserved
*  by Intel, and are designed to service exceptions! */
extern void _isr0();
extern void _isr1();
extern void _isr2();
extern void _isr3();
extern void _isr4();
extern void _isr5();
extern void _isr6();
extern void _isr7();
extern void _isr8();
extern void _isr9();
extern void _isr10();
extern void _isr11();
extern void _isr12();
extern void _isr13();
extern void _isr14();
extern void _isr15();
extern void _isr16();
extern void _isr17();
extern void _isr18();
extern void _isr19();
extern void _isr20();
extern void _isr21();
extern void _isr22();
extern void _isr23();
extern void _isr24();
extern void _isr25();
extern void _isr26();
extern void _isr27();
extern void _isr28();
extern void _isr29();
extern void _isr30();
extern void _isr31();

void isrs_install()
{
    idt_set_gate(0, _isr0, 0x08, 0x8E);
    idt_set_gate(1, _isr1, 0x08, 0x8E);
    idt_set_gate(2, _isr2, 0x08, 0x8E);
    idt_set_gate(3, _isr3, 0x08, 0x8E);
    idt_set_gate(4, _isr4, 0x08, 0x8E);
    idt_set_gate(5, _isr5, 0x08, 0x8E);
    idt_set_gate(6, _isr6, 0x08, 0x8E);
    idt_set_gate(7, _isr7, 0x08, 0x8E);

    idt_set_gate(8, _isr8, 0x08, 0x8E);
    idt_set_gate(9, _isr9, 0x08, 0x8E);
    idt_set_gate(10, _isr10, 0x08, 0x8E);
    idt_set_gate(11, _isr11, 0x08, 0x8E);
    idt_set_gate(12, _isr12, 0x08, 0x8E);
    idt_set_gate(13, _isr13, 0x08, 0x8E);
    idt_set_gate(14, _isr14, 0x08, 0x8E);
    idt_set_gate(15, _isr15, 0x08, 0x8E);

    idt_set_gate(16, _isr16, 0x08, 0x8E);
    idt_set_gate(17, _isr17, 0x08, 0x8E);
    idt_set_gate(18, _isr18, 0x08, 0x8E);
    idt_set_gate(19, _isr19, 0x08, 0x8E);
    idt_set_gate(20, _isr20, 0x08, 0x8E);
    idt_set_gate(21, _isr21, 0x08, 0x8E);
    idt_set_gate(22, _isr22, 0x08, 0x8E);
    idt_set_gate(23, _isr23, 0x08, 0x8E);

    idt_set_gate(24, _isr24, 0x08, 0x8E);
    idt_set_gate(25, _isr25, 0x08, 0x8E);
    idt_set_gate(26, _isr26, 0x08, 0x8E);
    idt_set_gate(27, _isr27, 0x08, 0x8E);
    idt_set_gate(28, _isr28, 0x08, 0x8E);
    idt_set_gate(29, _isr29, 0x08, 0x8E);
    idt_set_gate(30, _isr30, 0x08, 0x8E);
    idt_set_gate(31, _isr31, 0x08, 0x8E);
}

extern void _irq0();
extern void _irq1();
extern void _irq2();
extern void _irq3();
extern void _irq4();
extern void _irq5();
extern void _irq6();
extern void _irq7();
extern void _irq8();
extern void _irq9();
extern void _irq10();
extern void _irq11();
extern void _irq12();
extern void _irq13();
extern void _irq14();
extern void _irq15();


void irq_remap(void)
{
    outp(0x20, 0x11);
    outp(0xA0, 0x11);
    outp(0x21, 0x20);
    outp(0xA1, 0x28);
    outp(0x21, 0x04);
    outp(0xA1, 0x02);
    outp(0x21, 0x01);
    outp(0xA1, 0x01);
    outp(0x21, 0x0);
    outp(0xA1, 0x0);
}

void irq_install()
{
    irq_remap();

    idt_set_gate(32, _irq0, 0x08, 0x8E);
    idt_set_gate(33, _irq1, 0x08, 0x8E);
    idt_set_gate(34, _irq2, 0x08, 0x8E);
    idt_set_gate(35, _irq3, 0x08, 0x8E);
    idt_set_gate(36, _irq4, 0x08, 0x8E);
    idt_set_gate(37, _irq5, 0x08, 0x8E);
    idt_set_gate(38, _irq6, 0x08, 0x8E);
    idt_set_gate(39, _irq7, 0x08, 0x8E);
    idt_set_gate(40, _irq8, 0x08, 0x8E);
    idt_set_gate(41, _irq9, 0x08, 0x8E);
    idt_set_gate(42, _irq10, 0x08, 0x8E);
    idt_set_gate(43, _irq11, 0x08, 0x8E);
    idt_set_gate(44, _irq12, 0x08, 0x8E);
    idt_set_gate(45, _irq13, 0x08, 0x8E);
    idt_set_gate(46, _irq14, 0x08, 0x8E);
    idt_set_gate(47, _irq15, 0x08, 0x8E);
}


void InterruptDescriptorTable_Initialize()
{
	//terminal_writestring("Enabling interrupts! \n");
	idt_install();
	irq_install();
	isrs_install();

	__asm__ __volatile__ ("sti"); 

}

