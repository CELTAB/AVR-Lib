#ifndef USART_H
#define USART_H

/* USART - Universal Synchronous Asynchronous Receiver Transmitter */
/* Functions implementations to use the Programmable Serial USART of
 * the AVR microcontrollers. Tested only on the ATmega series but problaby works
 * on other models that have this feature.
 * This implementation is intented to be used only as asynchronous mode (UART),
 * but the name USART was kept due to the registers names and descriptions */

#include <avr/io.h>
#include <util/setbaud.h>

#define USART_enable_rx_interrupt UCSR0B |= _BV(RXCIE0)

#define USART_disable_rx_interrupt UCSR0B &= ~_BV(RXCIE0)

#define USART_enable_udr_interrupt UCSR0B |= _BV(UDRIE0)

#define USART_disable_udr_interrupt UCSR0B &= ~_BV(UDRIE0)

void USART_init(void);

void USART_transmit_byte(uint8_t data);

uint8_t USART_receive_byte(void);

void USART_print_byte(uint8_t byte);

void USART_print_string(const char string[]);

#endif
