#ifndef SPI_H
#define SPI_H

#include <avr/io.h>

#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__)
#include "spim328p.h"
#endif
#if defined (__AVR_ATtiny84__)
#include "spitn84.h"
#endif


/* The definitions of the CSN PORT are defined in
 * the mcu-specific files (like spim328.h). In
 * order to improve the performance in case of multiple
 * callings, it's up to the caller to toggle the CSN */

extern void SPI_init_master(void);

extern uint8_t SPI_transfer(uint8_t data);

void SPI_write_data(uint8_t *data, uint8_t length);

void SPI_read_data(uint8_t* data, uint8_t length);

#endif
