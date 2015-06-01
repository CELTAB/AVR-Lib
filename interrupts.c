#include "interrupts.h"

/* Enable the given External Interrupt pin
 * and set the Sense Control Mode */
void EINT_enable(uint8_t pin, uint8_t mode)
{
	EIMSK |= _BV(pin);
	/* The number of shift operation is multiplied by 2
	 * because the mode is 2-bit wide, in other words,
	 * each 2 bits control the interrupt mode for each pin */
	EICRA |= (mode<<(2*pin));
}

/* Disable External Interrupt on the given pin */
void EINT_disable(uint8_t pin)
{
	EIMSK &= ~_BV(pin);
}


