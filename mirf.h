#ifndef MIRF_H
#define MIRF_H

#include <avr/io.h>

#define CE_high	PORTB |= (1<<DDB1);
#define CE_low	PORTB &= ~(1<<DDB1);

uint8_t mirf_status(void);

uint8_t mirf_get_reg(uint8_t reg);

// Write one byte into the MiRF register
void mirf_config_register(uint8_t reg, uint8_t value);
	
// Reads an array of bytes from the MiRF registers.
void mirf_read_register(uint8_t reg, uint8_t *value, uint8_t len);

// Writes an array of bytes into the MiRF register
void mirf_write_register(uint8_t reg, uint8_t *value, uint8_t len);

void nrf24l01_init(void);

// Sends a data package to the default address. Be sure to send the correct
// amount of bytes as configured as payload on the receiver.
uint8_t mirf_send(uint8_t *value, uint8_t len, uint8_t *address);

void mirf_flush_rx_tx(void);
	
// Checks if MAX_RT has been reached
uint8_t mirf_max_rt_reached(void);

uint8_t mirf_data_ready(void);

void mirf_get_data(uint8_t *data, uint8_t buffersize);

#endif
