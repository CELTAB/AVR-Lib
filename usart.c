#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>
#include "usart.h"

void USART_init(void)
{
	/* UBRR -> USART Baud Rate Registers - 12 bits */

	/* 4 most significant bits */
	UBRR0H = UBRRH_VALUE;
	/* 8 least significant bits */	
	UBRR0L = UBRRL_VALUE;
	
#if USE_2X // USE_2X defined in <util/setbaud.h>
	/* Double speed mode */
	UCSR0A |= (1<<U2X0);
#else
	/* Normal mode */
	UCSR0A &= ~(1<<U2X0);
#endif

	/* Enable USART transmitter and receiver */
	UCSR0B = (1<<TXEN0) | (1<<RXEN0);
	/* 1-bit Stop Bit and 8-bit Character Size */
	UCSR0C = (3<<UCSZ00);
}

void USART_transmit_byte(uint8_t data)
{
	/* Wait for empty transmit buffer */
	while(!(UCSR0A & (1<<UDRE0)));
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

uint8_t USART_receive_byte(void)
{
	/* Wait for data to be received */
	while(!(UCSR0A & (1<<RXC0)));
	/* Get and return received data from buffer */
	return UDR0;
}
