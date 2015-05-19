#include "usart.h"

/* UCSRnX - USART Control and Status Register n X */
/* UDRn - USART I/O Data Register n */
/* Page 195 on the ATmega 48/88/128/328 datasheet
 * for more informations */

void USART_init(void)
{
	/* UBRR -> USART Baud Rate Registers - 12 bits */
    /* Set the baud rate calculated by <util/detbaud.h> */

	/* 4 most significant bits */
	UBRR0H = UBRRH_VALUE;
	/* 8 least significant bits */
	UBRR0L = UBRRL_VALUE;


    /* The <util/setbaud.h> checks if the baud rate achieved has a 2% max tolerance,
     * if not, the value is recalculated in order to use double speed mode, which
     * shall be set in the mcu register.
     * You can alter the max tolerance by #define BAUD_TOL */
#if USE_2X
	/* Double speed mode */
    /* Only allowed in asynchronous communication */
	UCSR0A |= _BV(U2X0);
#else
	/* Normal mode */
    /* Normally not used */
	UCSR0A &= ~(_BV(U2X0);
#endif

    /* UMSELn[1:0] - Mode Select */
    /* UPMn[1:0] - Parity Mode */
    /* USBSn - Stop Bit Select */
    /* Set the default configuration: asynchronous mode, parity disabled
     * and 1-bit stop bit */
    UCSR0C = 0;

    /* UCSZn[2:0] - USART Character Size */
    /* Set the size as 8-bit (also the default) */
    /* UCSZn2: 0 | UCSZn1: 1 | UCSZN2: 1 */
    UCSR0B &= ~(_BV(UCSZ02));
    UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);

	/* Enable USART transmitter and receiver */
	UCSR0B |= _BV(TXEN0) | _BV(RXEN0);
}

void USART_transmit_byte(uint8_t data)
{
    /* URDEn - USART Data Register Empty */
    /* The transmit buffer can only be written when it is empty,
     * therefore, the URDEn Flag mus be set */
	/* Wait for empty transmit buffer */
    loop_until_bit_is_set(UCSR0A, UDRE0);

	/* Put data into buffer, sends the data */
	UDR0 = data;
}

uint8_t USART_receive_byte(void)
{
    /* RXCn - USART Receive Complete */
    /* This flag is set where there are unread data in the
     * receiver buffer and cleared when it is empty */
	/* Wait for data to be received */
    loop_until_bit_is_set(UCSR0A, RXC0);

	/* Get and return received data from buffer */
	return UDR0;
}
