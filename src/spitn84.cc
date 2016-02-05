#include <avrutil/spitn84.h>

void SPI_init_master(void)
{
	/* Set CSN pin as output */
	CSN_DDR |= _BV(CSN);
	/* Initializate as disabled */
	CSN_high;

	/* Source Clock | Master out, Slave in */
	DDRA |= _BV(SCK) | _BV(MOSI);

	/* Master in, Slave Out */
	DDRA &= ~(_BV(MISO));
	/* Activate pull-up resistor */
	PORTA |= _BV(MISO);
}

uint8_t SPI_transfer(uint8_t data)
{
	// Load byte to data register
	USIDR = data;

	// Clear flag to be able to receive new data
	USISR |= (1<<USIOIF);

	USICR |= (1<<USIWM0) | (1<<USICS1) | (1<<USICLK);
	_delay_us(10);

	// Wait for transmission complete
	while((USISR & (1<<USIOIF)) == 0)
	{
		// Toggle SCK and count a 4-bit counter from 0-15,
		// when it reaches 15 USIOIF is set
		USICR |= (1<<USITC);
	}

	USICR &= ~((1<<USIWM0) | (1<<USICS1) | (1<<USICLK));
	_delay_us(10);

	return USIDR;
}
