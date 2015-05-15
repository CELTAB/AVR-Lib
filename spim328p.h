#ifndef SPI328P_H
#define SPI328P_H

#include <avr/io.h>
#include <util/delay.h>

/* Define a default CSN (Chip Select Not) */
/* Also known as SS (Slave Select) */
#define CSN_DDR DDRB
#define CSN_PORT PORTB
#define CSN PB2

#define MOSI DDB3
#define MISO DDB4
#define SCK DDB5

void SPI_init_master(void);

uint8_t SPI_transfer(uint8_t byte);

#endif
