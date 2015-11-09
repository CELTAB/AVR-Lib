#include "spi.h"

void SPI_write_data(uint8_t *data, uint8_t length)
{
	uint8_t i;
	for (i = 0; i < length; i++) {
		SPI_transfer(data[i]);
	}
}

void SPI_read_data(uint8_t *data, uint8_t length)
{
	uint8_t i;
	for (i = 0; i < length; i++) {
		data[i] = SPI_transfer(0xFF);
	}
}
