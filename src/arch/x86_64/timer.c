#include <system.h>

#define TIMER_PHASE_HZ 100 // 100 hz (hertz (hz) is the number of times something occurs per second)
#define TICKS_IN_MINUTES (TIMER_PHASE_HZ * 60)
#define TICKS_IN_HOUR (TICKS_IN_MINUTES * 60)
#define TICKS_IN_DAY (TICKS_IN_HOUR * 24)

#define TIMER_IRQ 32 // timer interrupt request slot

// How long has the system been running for? 
// 1 tick is 1/TIMER_PHASE_HZ-th of a second
int timer_ticks = 0;

void timer_phase(int hz)
{
    int divisor = 1193180 / hz;       /* Calculate our divisor */
    outp(0x43, 0x36);             /* Set our command byte 0x36 */
    outp(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    outp(0x40, divisor >> 8);     /* Set high byte of divisor */
}

void disable_timer()
{
	outp(0x21,0xfd);
	outp(0xa1,0xff);
}

// Returns time elapsed in seconds rounded down.
int time_in_seconds()
{
	return timer_ticks / TIMER_PHASE_HZ; 
}

int time_in_minutes()
{
	return timer_ticks / TICKS_IN_MINUTES; 
}

void output_totaltime_to_terminal()
{
	int hours = timer_ticks / TICKS_IN_HOUR;
	int minutes = (timer_ticks % TICKS_IN_HOUR) / TICKS_IN_MINUTES;
	int seconds =  (timer_ticks % TICKS_IN_MINUTES) / TIMER_PHASE_HZ;

	terminal_writestring("Time: "); terminal_writeint(hours); terminal_writestring(" hours ");
	terminal_writeint(minutes); terminal_writestring (" minutes ");
	terminal_writeint(seconds); terminal_writestring(" seconds \n");
}

void timer_interrupt_handler()
{
	timer_ticks++;
	
	int seconds = time_in_seconds();

	if (timer_ticks % TIMER_PHASE_HZ == 0)
	{
		output_totaltime_to_terminal();
	}
}

void setup_timer()
{
	terminal_writestring("Setting up timer!\n");
	timer_phase(TIMER_PHASE_HZ);
	register_handler(timer_interrupt_handler, TIMER_IRQ);
}

