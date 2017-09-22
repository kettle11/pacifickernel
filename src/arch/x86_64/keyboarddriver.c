#include <system.h>
#include <stdbool.h> /* C doesn't have booleans by default. */
#include "scancodes.h" // In a separate file because it's just a giant table.
#include "events.h" // KeyboardEvent is defined in events.h

#define KEYBOARD_IRQ 33 // IRQ stands for "Interrupt ReQuest". 33 is the IRQ slot that is changed when the keyboard has an event.
#define KEYBOARD_DATA_PORT 0x60 // The port the keyboard passes data on
#define KEYBOARD_STATUS_READ_PORT 0x64 
#define KEYBOARD_COMMAND_WRITE_PORT 0x64 

// Note this driver assumes operating in Scan Code Set 1 mode.
bool keydown[128];

void (*keypress_handler)(struct KeyboardEvent);

bool capitalize()
{
	bool shiftHeld = keydown[special_keys_us[5]] || keydown[special_keys_us[6]];
	return shiftHeld;
}

void produce_key_press_event(
	unsigned char scancode,
	bool pressed,
	bool repeat,
	bool released)
{
	if (keypress_handler == 0)
		return;

	struct KeyboardEvent newEvent;

	if (capitalize())
	{
		newEvent.character = to_upper(keyboard_us[scancode]);
	}
	else
	{
		newEvent.character = keyboard_us[scancode];
	}

	newEvent.scancode = scancode;
	newEvent.pressed = pressed;
	newEvent.released = released;
	newEvent.repeating = repeat;

	keypress_handler(newEvent);
}

void assign_key_press_handler(void (*keypress_handler_assign)(struct KeyboardEvent))
{
	keypress_handler = keypress_handler_assign;
}

/* This is the main function that interacts with the keyboard by listening for key pressed events */
void keyboard_interrupt_handler(void)
{
    unsigned char scancode;

    /* Read from the keyboard's data buffer */
    scancode = inb(KEYBOARD_DATA_PORT);

    // Scan code's greater than 0x80 are released keys. 
    if (scancode > 0x80)
    {
    	// Releases are 0x80 greater than their pressed counterpart.
    	scancode  = scancode - 0x80;	
    	produce_key_press_event(scancode,
    	 false  /*not pressed*/,
    	 false, /*not repeating*/
    	 true); /*released*/

    	keydown[scancode] = false;
    }
    else
    {
    	produce_key_press_event(scancode,
    	 true  				/*pressed*/,
    	 keydown[scancode], /*if it's already held down this is a repeat*/
    	 false); 			/*not released*/

    	keydown[scancode] = true;
    }
}

void setup_keyboard_driver()
{
	register_handler(keyboard_interrupt_handler, KEYBOARD_IRQ);
	memset(&keydown, false, sizeof(keydown));
	keypress_handler = 0;
}
