#ifndef USART_H
#define USART_H

#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>

#define MYUBRR F_CPU/16/BAUD-1
#define TESTE (F_CPU/(16*BAUD))-1

void USART_init(void);

void USART_transmit_byte(uint8_t data);

uint8_t USART_receive_byte(void);

#endif
