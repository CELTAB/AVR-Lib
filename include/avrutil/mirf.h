#ifndef MIRF_H
#define MIRF_H

#include <avr/io.h>
#include <util/delay.h>
#include <avrutil/nrf24l01.h>

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
/* 1000 us retransmit delay and 15 retransmit count */
#define MIRF_RETR (5<<ARD | 15<<ARC)
#endif

#ifndef MIRF_CH
/* 2.4 + 0.042 GHz frequency */
#define MIRF_CH 42
#endif

#ifndef MIRF_SETUP
/* RF_DR - Data Rate ('0' - 1 Mbps | '1' - 2 Mbps) */
/* RF_PWR - Output power in TX mode */
/* 1 Mpbs data rate and 0dBm output power */
#define MIRF_SETUP (1<<RF_DR | 3<<RF_PWR)
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
#define MIRF_POWER_UP MIRF_set_register_bit(CONFIG, PWR_UP);\
	_delay_ms(4)

#define MIRF_POWER_DOWN MIRF_clear_register_bit(CONFIG, PWR_UP)

/* CE_low before entering TX mode to prevent unwanted transmission */
#define MIRF_TX CE_low; MIRF_clear_register_bit(CONFIG, PRIM_RX);\
	_delay_us(130)

#define MIRF_RX MIRF_set_register_bit(CONFIG, PRIM_RX);\
	CE_high; _delay_us(130)

#define MIRF_STANDBY_ON CE_low;

#define MIRF_STANDBY_OFF CE_high; _delay_us(130)
/* --------------------------------------- */

#define MIRF_MAX_RT_REACHED MIRF_status() & (1<<MAX_RT)
#define MIRF_DATA_READY MIRF_status() & (1<<RX_DR)
#define MIRF_DATA_SENT MIRF_status() & (1<<TX_DS)

#define MIRF_CLEAR_MAX_RT MIRF_set_register(STATUS, 1<<MAX_RT);
#define MIRF_CLEAR_RX_DR MIRF_set_register(STATUS, 1<<RX_DR);
#define MIRF_CLEAR_TX_DS MIRF_set_register(STATUS, 1<<TX_DS);

/* Set the configuration registers with the defined values
 * and the CE pin as output */
void MIRF_setup_config(void);

/* Clear the interrupt flags, the RX and TX FIFOs and
 * enter in the Standby-I mode */
void MIRF_init(void);

/* Set the address on the given pipe */
void MIRF_set_address(uint8_t pipe,  uint8_t* address);

/* Enable given RX data pipe[5:0] with auto acknowledgment
 * and set the RX address. Be aware that the pipes 1 to 5 share the
 * same 4 most signifcant bytes.
 * If ack_pay is different than zero, it will enable the dynamic payload
 * and ACK payload features.
 * If ack_pay and payload_size are equal to zero, it will enable just the
 * dynamic payload feature.
 * If ack_pay is zero and payload_size is different than zero, it will set
 * the given RX pipe payload_size (0 to 32 bytes) */
void MIRF_enable_rx_pipe(uint8_t pipe, uint8_t payload_size, uint8_t* address);

/* Disable given RX pipe [5:0] */
void MIRF_disable_rx_pipe(uint8_t pipe);

/* Enable dynamic payload feature */
void MIRF_enable_dynamic_payload(void);

/* Disable dynamic payload feature */
void MIRF_disable_dynamic_payload(void);

/* Enable ack payload feature */
void MIRF_enable_ack_payload(void);

/* Disable ack payload feature */
void MIRF_disable_ack_payload(void);

/* Read the Status Register */
uint8_t MIRF_status(void);

/* Read one byte from the MiRF register */
uint8_t MIRF_get_register(uint8_t reg);

/* Write one byte into the MiRF register */
void MIRF_set_register(uint8_t reg, uint8_t value);

/* Read one byte from the MiRF register and mask
 * with the given mnemonic */
uint8_t MIRF_get_register_bit(uint8_t reg, uint8_t mnemonic);

/* Write one byte masked with the given mnemonic into
 * the MiRF register */
void MIRF_set_register_bit(uint8_t reg, uint8_t mnemonic);

/* Write one byte reversed masked with the given mnemonic into
 * the MiRF register */
void MIRF_clear_register_bit(uint8_t reg, uint8_t mnemonic);

/* Read an array of bytes from the MiRF register */
void MIRF_read_register(uint8_t reg, uint8_t *value, uint8_t len);

/* Write an array of bytes into the MiRF register */
void MIRF_write_register(uint8_t reg, uint8_t *value, uint8_t len);

/* Write an array of bytes into the W_ACK_PAYLOAD  register with the
 * given pipe */
void MIRF_write_ack_payload(uint8_t pipe, uint8_t *value, uint8_t len);

/* Send a data package to the given address. Be sure to send the
 * correct amount of bytes as configured as payload on the receiver.
 * This method also takes care of the RX[0] status and handles the
 * nRF IRQ flags.
 * Returns 1 if data transmission is successful and 0 if not */
uint8_t MIRF_send_data_no_irq(uint8_t *address, uint8_t *data, uint8_t payload_size);

/* Send a data package to the given address. Be sure to send the
 * correct amount of bytes as configured as payload on the receiver
 * It's up to the caller to handle the nRF IRQ flags */
void MIRF_send_data(uint8_t *address, uint8_t *data, uint8_t payload_size);

/* Transmit the TX FIFO content within given delay in
 * microseconds. Enter in Standby-II mode during this
 * period if the TX FIFO is empty */
void MIRF_transmit(void);

/* Flush the RX and TX FIFOs */
void MIRF_flush_rx_tx(void);

/* Flush the RX FIFO */
void MIRF_flush_rx(void);

/* Flush the TX FIFO */
void MIRF_flush_tx(void);

/* Wait for data and read it when it arrives */
void MIRF_receive_data(uint8_t *data, uint8_t payload_size);

/* Read the RX payload */
void MIRF_read_data(uint8_t *data, uint8_t payload_size);

/* Read the top RX payload size and then read the
 * RX payload with the read value */
uint8_t MIRF_read_dynamic_payload_data(uint8_t *data);

/* Clear the flags for the RX_DR, TX_DS and MAX_RT interrupts */
void MIRF_clear_all_interrupts(void);

#endif
