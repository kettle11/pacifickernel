#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h> /* C doesn't have booleans by default. */


/* MAIN.C */
extern void *memcpy(void *dest, const void *src, size_t count);
extern void *memset(void *dest, char val, size_t count);
extern unsigned short *memsetw(void *dest, unsigned short val, size_t count);
extern size_t strlen(const char *str);
extern void outp(uint16_t port, uint8_t val);
extern uint8_t inb(uint16_t port);

extern void register_handler(void(*interrupt_handler), int index);
extern void unregister_handler(int index);
extern char to_lower(char character);
extern char to_upper(char character);
#endif