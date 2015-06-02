#ifndef MIRF_H
#define MIRF_H

#include <avr/io.h>
#include <util/delay.h>
#include "nrf24l01.h"

#ifndef CE
#define CE DDB1
#endif
#ifndef CE_PORT
#define CE_PORT PORTB
#endif
#ifndef CE_DDR
#define CE_DDR DDRB
#endif

#define CE_high	CE_PORT |= (1<<CE);
#define CE_low CE_PORT &= ~(1<<CE);

#ifndef ADDR_SIZE
/* Must be the same as defined in MIRF_AW */
#define ADDR_SIZE 5
#endif

/* --------------------------------------- */
/* Definitions for the setup configuration */
#ifndef MIRF_AW
/* '01' - 3 bytes | '10' - 4 bytes | '11' - 5 bytes (default) */
/* 5 bytes address */
#define MIRF_AW (3<<AW)
#endif

#ifndef MIRF_RETR
/* ARD - Auto Retransmit Delay */
/* ARC - Auto Retransmit Count */
/* 250+86uS retransmit delay and 3 retransmit count */
#define MIRF_RETR (0<<ARD | 3<<ARC)
#endif

#ifndef MIRF_CH
/* 2.4 + 0.042 GHz frequency */
#define MIRF_CH 42
#endif

#ifndef MIRF_SETUP
/* RF_DR - Data Rate ('0' - 1 Mpbs | '1' - 2 Mpbs) */
/* RF_PWR - Output power in TX mode */
/* 1 Mpbs data rate and 0dBm output power */
#define MIRF_SETUP (0<<RF_DR | 3<<RF_PWR)
#endif

#ifndef MIRF_CONFIG
/* MASK_RX_DR - Mask Interrupt caused by RX_DR */
/* MASK_TX_DS - Mask Interrupt caused by TX_DS */
/* MASK_MAX_RT - Mask Interrupt caused by MAX_RT */
/* EN_CRC - Enable CRC */
/* CRCO - CRC encoding scheme ('0' - 1 byte | '1' - 2 bytes */
/* All interrupts reflected on IRQ pin, CRC enabled and 1 byte encoding scheme */
#define MIRF_CONFIG (0<<MASK_RX_DR | 0<<MASK_TX_DS | 0<<MASK_MAX_RT | 1<<EN_CRC | 0<<CRCO)
#endif
/* --------------------------------------- */

/* --------------------------------------- */
/* The nRF24L01 must always settle in Standby for 1.5ms
 * before it cant enter on the TX or RX modes */
#define MIRF_TX_POWER_UP MIRF_set_register(CONFIG, (MIRF_CONFIG | (1<<PWR_UP)) & ~(1<<PRIM_RX));\
	_delay_ms(5);CE_low

#define MIRF_RX_POWER_UP MIRF_set_register(CONFIG, MIRF_CONFIG | (1<<PWR_UP) | (1<<PRIM_RX));\
	_delay_ms(5);CE_high

#define MIRF_POWER_DOWN MIRF_set_register(CONFIG, MIRF_CONFIG & ~(1<<PWR_UP))
/* --------------------------------------- */

#define MIRF_MAX_RT_REACHED MIRF_status() & (1<<MAX_RT)
#define MIRF_DATA_READY MIRF_status() & (1<<RX_DR)
#define MIRF_DATA_SENT MIRF_status() & (1<<TX_DS)

/* Set the defined configurations and set the CE pin as output */
void MIRF_setup_configuration(void);

/* Enable given RX data pipe [5:0] with auto acknowledgment,
 * set its payload_size[32:0](bytes) and set the RX
 * address. Be aware that the pipes [5:1] share the
 * same 4 most signifcant bytes */
void MIRF_enable_rx_pipe(uint8_t pipe, uint8_t payload_size, uint8_t* address);

/* Disable given RX pipe [5:0] */
void MIRF_disable_rx_pipe(uint8_t pipe);

/* Read the Status Register */
uint8_t MIRF_status(void);

/* Read one byte from the MiRF register */
uint8_t MIRF_get_register(uint8_t reg);

/* Write one byte into the MiRF register */
void MIRF_set_register(uint8_t reg, uint8_t value);

/* Read an array of bytes from the MiRF register */
void MIRF_read_register(uint8_t reg, uint8_t *value, uint8_t len);

/* Write an array of bytes into the MiRF register */
void MIRF_write_register(uint8_t reg, uint8_t *value, uint8_t len);

/* Send a data package to the given address. Be sure to send the
 * correct amount of bytes as configured as payload on the receiver
 * Returns 1 if data transmission is successful and 0 if not */
uint8_t MIRF_send_data(uint8_t *address, uint8_t *data, uint8_t payload_size);

/* Flush the RX and TX payloads */
void MIRF_flush_rx_tx(void);

/* Wait for data and read it when it arrives */
void MIRF_receive_data(uint8_t *data, uint8_t payload_size);

void MIRF_clear_interrupt(void);

#endif
