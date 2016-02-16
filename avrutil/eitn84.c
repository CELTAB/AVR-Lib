#include "eitn84.h"

/* Enable the given External Interrupt pin
 * and set the Sense Control Mode */
void EINT_enable(uint8_t intx, uint8_t mode)
{
	GIMSK |= _BV(intx);
	/* Clear the bits and then set them */
	MCUCR &= ~(3);
	MCUCR |= mode;
}

/* Disable External Interrupt on the given pin */
void EINT_disable(uint8_t intx)
{
	GIMSK &= ~_BV(intx);
}
