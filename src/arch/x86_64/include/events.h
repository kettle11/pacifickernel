#ifndef __EVENTS_H
#define __EVENTS_H

struct KeyboardEvent
{
	char character;
	char scancode;
	
	bool pressed;
	bool repeating; // Is this a repeat from a held key? 

	bool released;
};

#endif