#ifndef EXT_INTERRUPT_H
#define EXT_INTERRUPT_H

#include <avr/io.h>
#include <avr/interrupt.h>

#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__)
#include "eim328p.h"
#endif
#if defined (__AVR_ATtiny84__)
#include "eitn84.h"
#endif

/* External Interrupt Sense Control Mnemonics */
#define EISC_LOW_LEVEL 0
#define EISC_ANY_LOGICAL_CHANGE 1
#define EISC_FALLING_EDGE 2
#define EISC_RISING_EDGE 3

/* Enable the given External Interrupt pin
 * and set the Sense Control Mode */
extern void EINT_enable(uint8_t intx, uint8_t mode);

/* Disable External Interrupt on the given pin */
extern void EINT_disable(uint8_t intx);

#endif
