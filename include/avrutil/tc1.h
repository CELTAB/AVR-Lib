#include <avr/io.h>

#define MAX 65536

#define NO_CLK		0
#define CLK_1		1
#define CLK_8		2
#define CLK_64		3
#define CLK_256		4
#define CLK_1024	5
/* TODO: not sure if works with
 * external clock. Test it */
#define EXT_CLK_FALL	6
#define EXT_CLK_RISE	7

uint16_t volatile overflow_count, max_overflow;

ISR(TIMER1_OVF_vect)
{
	if(++overflow_count >= max_overflow) {
		overflow_count = 0;
		/* If there are remaining clocks to count,
		** clear the Output Compare A interrupt flag */
		if(OCIE1A) {
			TIFR1 |= (1 << OCF1A);
		}
		/* Enable interrupt on Output Compare A */
		TIMSK1 = (1 << OCIE1A);
	}
}

/* If using external clock prescale, clk_io should be the according
 * input value, otherwise, it should be the same as F_CPU */
void TC1_start(uint32_t clk, uint8_t clk_prescale, uint32_t time_ms);

void TC1_stop(void);

/* To be called on remainder ticks IRQ handler,
 * normally TIMER1_COMPA_vect */
void TC1_restart(void);
