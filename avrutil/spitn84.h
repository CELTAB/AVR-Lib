/* This file should be included from <spi.h> */
#ifndef SPITN84_H
#define SPITN84_H

#include <avr/io.h>
#include <util/delay.h>

/* Define a default CSN (Chip Select Not) */
/* Also known as SS (Slave Select) */
#ifndef CSN_DDR
#define CSN_DDR DDRA
#endif

#ifndef CSN_PORT
#define CSN_PORT PORTA
#endif

#ifndef CSN
#define CSN DDA7
#endif

/* Transmission disabled on this port */
#define CSN_high	CSN_PORT |=  _BV(CSN);
/* Transmission enabled on this port */
#define CSN_low		CSN_PORT &= ~(_BV(CSN));

#define MOSI DDA5
#define MISO DDA6
#define SCK  DDA4

void SPI_init_master(void);

uint8_t SPI_transfer(uint8_t byte);

#endif
