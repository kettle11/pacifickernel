#include <stdint.h>

#define PRESENT     			0x1
#define WRITABLE    			0x10
#define USERACCESS 				0x100
#define WRITETHROUGHCACHING 	0x1000
#define DISABLECACHE		 	0x10000
#define ACCESSED			 	0x100000
#define DIRTY				 	0x1000000
#define HUGE		 			0x10000000
#define GLOBAL				 	0x100000000

// This file is 64-bit centric.
// Good explanation at this tutorial: https://os.phil-opp.com/entering-longmode/

// PRESENTLY THIS FILE IS INCOMPLETE AND DOES NOT WORK AT ALL

extern const void kernel_end;

extern uint64_t p4_table[512];
extern uint64_t p3_table[512];
extern uint64_t p2_table[512];
// "p1" is the actual memory

void map_entry(uint64_t* table, uint16_t location, uint64_t address, uint16_t flags)
{
	table[location] = address | flags;
}

void create_page(void* parentTable, uint16_t location, uint16_t flags)
{

}

void map_page_tables(void)
{
	map_entry(p4_table, 0, p3_table, PRESENT | WRITABLE);
	map_entry(p3_table, 0, p2_table, PRESENT | WRITABLE);

	for (int i = 0; i < 512; i++)
	{
		// Each entry is 2mb.
		map_entry(p2_table, i, 0x200000 * i, PRESENT | WRITABLE | HUGE);
	}
}
