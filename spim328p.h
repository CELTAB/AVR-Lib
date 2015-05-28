/* This file should be included from <spi.h> */

#ifndef SPI328P_H
#define SPI328P_H

#include <avr/io.h>
#include <util/delay.h>

/* Define a default CSN (Chip Select Not) */
/* Also known as SS (Slave Select) */
#ifndef CSN_DDR
#define CSN_DDR DDRB
#endif

#ifndef CSN_PORT
#define CSN_PORT PORTB
#endif

#ifndef CSN
#define CSN PB2
#endif

/* Transmission disabled on this port */
#define CSN_high	CSN_PORT |=  _BV(CSN);
/* Transmission enabled on this port */
#define CSN_low		CSN_PORT &= ~(_BV(CSN));

#define MOSI DDB3
#define MISO DDB4
#define SCK DDB5

void SPI_init_master(void);

uint8_t SPI_transfer(uint8_t byte);

#endif
