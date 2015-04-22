#ifndef SPI_328P_H
#define SPI_328P_H

#include <avr/io.h>
#include <util/delay.h>
#include "nrf24l01.h"

// Definitions for selecting and enabling MiRF module
#define CSN_high	PORTB |=  (1<<DDB2);
#define CSN_low		PORTB &= ~(1<<DDB2);
#define CE_high		PORTB |=  (1<<DDB1);
#define CE_low		PORTB &= ~(1<<DDB1);

void init_spi(void);

uint8_t spi_transfer(uint8_t data);

// Write data using SPI
void spi_write_data(uint8_t *dataout, uint8_t len);
		
// Read data using SPI
void spi_read_data(uint8_t *datain, uint8_t len);

#endif
