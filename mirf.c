#include <avr/io.h>
#include <util/delay.h>
#include "nrf24l01.h"
#include "mirf.h"
#include "spi_m328p.h"

#define TADDR 		(uint8_t *)"taddr"
#define RADDR		(uint8_t *)"raddr"

uint8_t mirf_status(void)
{
	_delay_us(10);
	CSN_low; // Pull down chip select
	_delay_us(10);
	spi_transfer(R_REGISTER | (REGISTER_MASK & STATUS));
	_delay_us(10);
	uint8_t status = spi_transfer(NOP); // Read status register
	CSN_high; // Pull up chip select

	return status;
}

uint8_t mirf_get_reg(uint8_t reg)
{
	_delay_us(10);
	CSN_low; // Pull down chip select
	_delay_us(10);
	spi_transfer(R_REGISTER | reg);
	_delay_us(10);
	reg = spi_transfer(NOP); // Read status register
	CSN_high; // Pull up chip select

	return reg;
}

// Write one byte into the MiRF register
void mirf_config_register(uint8_t reg, uint8_t value)
{
	CSN_low;
	spi_transfer(W_REGISTER | (REGISTER_MASK & reg));
	spi_transfer(value);
	CSN_high;
}

// Reads an array of bytes from the MiRF registers.
void mirf_read_register(uint8_t reg, uint8_t *value, uint8_t len)
{
	CSN_low;
	spi_transfer(R_REGISTER | (REGISTER_MASK & reg));
	spi_read_data(value, len);
	CSN_high;
}

// Writes an array of bytes into the MiRF register
void mirf_write_register(uint8_t reg, uint8_t *value, uint8_t len)
{
	CSN_low;
	spi_transfer(W_REGISTER | (REGISTER_MASK & reg));
	spi_write_data(value, len);
	CSN_high;
}

void nrf24l01_init(void)
{
	_delay_ms(100);	//allow radio to reach power down if shut down

	//mirf_config_register(SETUP_RETR, 0xFF);
	
	mirf_config_register(EN_RXADDR, 1<<ERX_P0); //enable data pipe 0
	
	mirf_config_register(RX_PW_P0, 1); // set number of bytes in RX payload

	/*val[0]=0x03;
	read_write_nrf(W, SETUP_AW, val, 1); //0b0000 00011 = 5 bits*/

	/*val[0]=0x01;
	read_write_nrf(W, RF_CH, val, 1); //RF channel registry 0b0000 0001 = 2,401GHz*/

	/*val[0]=0x07;
	read_write_nrf(W, RF_SETUP, val, 1); //0b00000111*/

	uint8_t mirf_config = 10;// 0b1010
	mirf_config_register(CONFIG, mirf_config);

	//device need 1.5ms to reach standby mode
	_delay_ms(100);	
}

// Sends a data package to the default address. Be sure to send the correct
// amount of bytes as configured as payload on the receiver.
uint8_t mirf_send(uint8_t *value, uint8_t len, uint8_t *address)
{
	CSN_low; // Pull down chip select
	spi_transfer(FLUSH_TX);  // Write cmd to flush tx fifo
	CSN_high; // Pull up chip select

	// set the adress of the receiving chip
	mirf_write_register(RX_ADDR_P0, address, 5);
	mirf_write_register(TX_ADDR, address, 5);
	
	CSN_low;  // Pull down chip select
	spi_transfer(W_TX_PAYLOAD); // Write cmd to write payload
	spi_write_data(value, len); // Write payload
	CSN_high; // Pull up chip select
	
	CE_high; // Start transmission
	_delay_us(20);
	CE_low;

	// If maximum retries were reached, reset MAX_RT
	if(mirf_max_rt_reached()) {
		mirf_config_register(STATUS, 1<<MAX_RT);
		return 0;
	} else {
		return 1;
	}
}

void mirf_flush_rx_tx(void)
{
	CSN_low; // Pull down chip select
	spi_transfer(FLUSH_RX); // Flush RX
	CSN_high; // Pull up chip select
	CSN_low; // Pull down chip select
	spi_transfer(FLUSH_TX);  // Write cmd to flush tx fifo
	CSN_high; // Pull up chip select
}

// Checks if MAX_RT has been reached
uint8_t mirf_max_rt_reached(void)
{
	CSN_low; // Pull down chip select
	spi_transfer(R_REGISTER | (REGISTER_MASK & STATUS));
	uint8_t status = spi_transfer(NOP); // Read status register
	CSN_high; // Pull up chip select
	return status & (1<<MAX_RT);
}

uint8_t mirf_data_ready(void)
{
	CSN_low;

	spi_transfer(R_REGISTER | (REGISTER_MASK & STATUS));
	uint8_t status = spi_transfer(NOP);
	CSN_high;
	return status & (1<<RX_DR);
}

void mirf_get_data(uint8_t *data, uint8_t buffersize)
{
	CSN_low;
	spi_transfer(R_RX_PAYLOAD);
	spi_read_data(data, buffersize);
	CSN_high;
	mirf_config_register(STATUS, (1<<RX_DR));
}
