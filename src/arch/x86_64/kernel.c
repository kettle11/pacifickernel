#include <stddef.h>
#include <stdint.h>

void kernel_main(void) {

	/* Initialize terminal interface */
	terminal_initialize();

	terminal_writestring("Welcome to PacificOS\n");
	terminal_writestring("~~~~~~~~~~~~~~~~~~~~\n");
	terminal_writestring("~~~~~~~~~~~~~~~~~~~~\n");

	/* setup the system timer! */
	setup_timer();

	/* Setup interrupts! */ 
	InterruptDescriptorTable_Initialize();

	/* Setup keyboard driver */
	setup_keyboard_driver();

	/* Setup VGA text interface (depends on keyboard driver) */
	vga_setup();

	/* Does nothing yet. */
	map_page_tables();

	// Do nothing after setup
	for (;;)
	{
		asm("hlt");
	}
}