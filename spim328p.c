#include "spim328p.h"

void SPI_init_master(void)
{
    /* Set CSN pin as output */
    CSN_DDR |= _BV(CSN);
    /* Initializate as disabled */
    CSN_high;

    /* Source Clock | Master out, Slave in */
	DDRB |= _BV(SCK) | _BV(MOSI);

    /* Master in, Slave Out */
	DDRB &= ~(_BV(MISO));
    /* Activate pull-up resistor */
	PORTB |= _BV(MISO);

    /* SPCR - SPI Control Register */
    /* MSTR: 1 - Master | 0 - Slave (default: 0) */
    /* SPE - SPI Enable */
	SPCR |= _BV(SPE) | _BV(MSTR);
}

uint8_t SPI_transfer(uint8_t byte)
{
	/* Load byte to SPI Data Register and start transmission */
	SPDR = byte;

    /* SPRS - SPI Status Register */
    /* SPIF - SPI Interrupt Flag */
    /* When the transfer is complete, the SPIF is set */
    loop_until_bit_is_set(SPSR, SPIF);

	_delay_us(10);

    /* return the received byte during transmission */
	return SPDR;
}
