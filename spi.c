#include "spi.h"

void SPI_write_data(uint8_t *dataout, uint8_t length)
{
	uint8_t i;
	for (i = 0; i < length; i++) {
		SPI_transfer(dataout[i]);
	}
}

void SPI_read_data(uint8_t *datain, uint8_t length)
{
	uint8_t i;
	for (i = 0; i < length; i++) {
		datain[i] = SPI_transfer(DUMMY);
	}
}
