#ifndef USART_H
#define USART_H

/* USART - Universal Synchronous Asynchronous Receiver Transmitter */
/* Functions implementations to use the Programmable Serial USART of
 * the AVR microcontrollers. Tested only on the ATmega series.
 * This implementation is intented to use only the asynchronous mode (UART),
 * but the name USART is maintained because of the registers names descriptions */

#include <avr/io.h>
#include <util/setbaud.h>

void USART_init(void);

void USART_transmit_byte(uint8_t data);

uint8_t USART_receive_byte(void);

#endif
