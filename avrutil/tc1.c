#include "tc1.h"

/* If using external clock prescale, clk_io should be the input value,
 * otherwise, it should be the same as F_CPU */
void TC1_start(uint32_t clk, uint8_t clk_prescale, uint32_t time_ms)
{
	/* clocks per milliseconds */
	float timer_clk;
	switch(clk_prescale) {
	case 0:
		return;
	case 1:
		timer_clk = clk/(1000 * 1.0);
		break;
	case 2:
		timer_clk = clk/(1000 * 8.0);
		break;
	case 3:
		timer_clk = clk/(1000 * 64.0);
		break;
	case 4:
		timer_clk = clk/(1000 * 256.0);
		break;
	case 5:
		timer_clk = clk/(1000 * 1024.0);
		break;
	default:
		timer_clk = (float)clk;
	}

	uint32_t total_tick = timer_clk * time_ms;
	max_overflow = total_tick / MAX;
	overflow_count = 0;

	/* Save interrupts status and disable it */
	unsigned char sreg = SREG;
	cli();
	/* Calculate remainder ticks after max overflow */
	OCR1A = (total_tick - max_overflow * MAX);

	/* Force default config */
	TCCR1A = 0;
	TIFR1 = 255;

	if(max_overflow == 0) {
		/* Enable remainder ticks interrupt */
		TIMSK1 = (1 << OCIE1A);
	} else {
		/* Enable overflow interrupt */
		TIMSK1 = (1 << TOIE1);
	}

	/* Restore interrupt status */
	SREG = sreg;
	/* Start timer */
	TCCR1B = (7 & clk_prescale);
}

void TC1_stop(void)
{
	TCCR1B = NO_CLK;
}

/* To be called on remainder ticks IRQ handler,
 * normally TIMER1_COMPA_vect */
void TC1_restart(void)
{
	if(max_overflow != 0) {
		TIMSK1 = (1 << TOIE1);
	}
	TCNT1 = 0;
}
