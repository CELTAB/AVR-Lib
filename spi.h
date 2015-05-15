#ifndef SPI_H
#define SPI_H

#include <avr/io.h>

#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__)
#   include "spim328p.h"
#endif

#define DUMMY 0xFF

extern void SPI_init_master(void);

extern uint8_t SPI_transfer(uint8_t data);

void SPI_write_data(uint8_t *dataout, uint8_t length);

void SPI_read_data(uint8_t *datain, uint8_t length);

#endif
