#include "spi_m328p.h"

void init_spi(void)
{
	//	   SCK    |    MOSI   |    CSN     |    CE as outuput
	DDRB |= (1<<DDB5) | (1<<DDB3) | (1<<DDB2) | (1<<DDB1);
	// MISO
	DDRB &= ~(1<<DDB4);
	PORTB |= (1<<DDB4);
	
	SPCR |= (1<<SPE) | (1<<MSTR);

	CSN_high; //CSN High -> Disabled
	
	CE_low;	//CE Low -> Disabled 
}

uint8_t spi_transfer(uint8_t data)
{	
	// Load byte to data register
	SPDR = data;

	// Wait for transmission complete
	while((SPSR & (1<<SPIF)) == 0);

	_delay_us(10);

	return SPDR;
}

// Write data using SPI
void spi_write_data(uint8_t *dataout, uint8_t len)
{
	uint8_t i;
	for (i = 0; i < len; i++) {
		spi_transfer(dataout[i]);
	}
}

// Read data using SPI
void spi_read_data(uint8_t *datain, uint8_t len)
{
	uint8_t i;
	for (i = 0; i < len; i++) {
		datain[i] = spi_transfer(NOP);
	}
}
