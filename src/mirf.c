#include <avrutil/mirf.h>
#include <avrutil/spi.h>
#include <avr/interrupt.h>

/* Set the configuration registers with the defined values
 * and the CE pin as output */
void MIRF_setup_config(void)
{
	SPI_init_master();

	/* Small delay to be sure that the nRF24L01 module is turned on */
	_delay_ms(100);

	/* SETUP_AW - Setup of Address Width (commom for all data pipes) */
	MIRF_set_register(SETUP_AW, MIRF_AW);

	/* SETUP_RETR - Setup of Automatic Retransmission */
	MIRF_set_register(SETUP_RETR, MIRF_RETR);

	/* RF_CH - Radio Frequency Channel */
	MIRF_set_register(RF_CH, MIRF_CH);

	/* RF_Setup - Radio Frequency Setup Register */
	MIRF_set_register(RF_SETUP, MIRF_SETUP);

	/* CONFIG - Configuration Register */
	MIRF_set_register(CONFIG, MIRF_CONFIG);

	/* Set CE pin as output */
	CE_DDR |= _BV(CE);
}

/* Clear the interrupt flags, the RX and TX FIFOs and
 * enter in the Standby-I mode */
void MIRF_init(void)
{
	MIRF_flush_rx_tx();
	MIRF_clear_all_interrupts();
	CE_low;
	MIRF_POWER_UP;
}
/* Keep record of the current pipes status
 * Pipes 0 and 1 are activated by default */
static volatile uint8_t _mask_pipe = 3;
/* Enable given RX data pipe[5:0] with auto acknowledgment,
 * set its payload_size (0 to 32 bytes) and set the RX
 * address. Be aware that the pipes 1 to 5 share the
 * same 4 most signifcant bytes */
void MIRF_enable_rx_pipe(uint8_t pipe, uint8_t payload_size, uint8_t* address)
{
	_mask_pipe |= _BV(pipe);
	/* EN_RXADDR - Enabled RX Addressess [5:0] */
	MIRF_set_register(EN_RXADDR, _mask_pipe);
	/* EN_AA - Enable Auto Acknowledgement Function [5:0] */
	MIRF_set_register(EN_AA, _mask_pipe);
	/* Set the payload_size and the address in the respective pipe */
	switch (pipe) {
		case 0:
			MIRF_set_register(RX_PW_P0, payload_size);
			MIRF_write_register(RX_ADDR_P0, address, ADDR_SIZE);
			break;
		case 1:
			MIRF_set_register(RX_PW_P1, payload_size);
			MIRF_write_register(RX_ADDR_P1, address, ADDR_SIZE);
			break;
		case 2:
			MIRF_set_register(RX_PW_P2, payload_size);
			MIRF_write_register(RX_ADDR_P2, address, ADDR_SIZE);
			break;
		case 3:
			MIRF_set_register(RX_PW_P3, payload_size);
			MIRF_write_register(RX_ADDR_P3, address, ADDR_SIZE);
			break;
		case 4:
			MIRF_set_register(RX_PW_P4, payload_size);
			MIRF_write_register(RX_ADDR_P4, address, ADDR_SIZE);
			break;
		case 5:
			MIRF_set_register(RX_PW_P5, payload_size);
			MIRF_write_register(RX_ADDR_P5, address, ADDR_SIZE);
			break;
	}
}

/* Disable given RX pipe [5:0] */
void MIRF_disable_rx_pipe(uint8_t pipe)
{
	_mask_pipe &= ~_BV(pipe);
	MIRF_set_register(EN_RXADDR, _mask_pipe);
}

/* Read the Status Register */
uint8_t MIRF_status(void)
{
	CSN_low;
	SPI_transfer(R_REGISTER | (REGISTER_MASK & STATUS));
	uint8_t status = SPI_transfer(NOP);
	CSN_high;

	return status;
}

/* Read one byte from MiRF register */
uint8_t MIRF_get_register(uint8_t reg)
{
	CSN_low;
	SPI_transfer(R_REGISTER | (REGISTER_MASK & reg));
	reg = SPI_transfer(NOP);
	CSN_high;

	return reg;
}

/* Write one byte into the MiRF register */
void MIRF_set_register(uint8_t reg, uint8_t value)
{
	CSN_low;
	SPI_transfer(W_REGISTER | (REGISTER_MASK & reg));
	SPI_transfer(value);
	CSN_high;
}

/* Read one byte from the MiRF register and mask
 * with the given mnemonic */
uint8_t MIRF_get_register_bit(uint8_t reg, uint8_t mnemonic)
{
	return MIRF_get_register(reg) & (1<<mnemonic);
}

/* Write one byte masked with the given mnemonic into
 * the MiRF register */
void MIRF_set_register_bit(uint8_t reg, uint8_t mnemonic)
{
	MIRF_set_register(reg, MIRF_get_register(reg) | (1<<mnemonic));
}

/* Write one byte reversed masked with the given mnemonic into
 * the MiRF register */
void MIRF_clear_register_bit(uint8_t reg, uint8_t mnemonic)
{
	MIRF_set_register(reg, MIRF_get_register(reg) & ~(1<<mnemonic));
}

/* Read an array of bytes from the MiRF register */
void MIRF_read_register(uint8_t reg, uint8_t *value, uint8_t len)
{
	CSN_low;
	SPI_transfer(R_REGISTER | (REGISTER_MASK & reg));
	SPI_read_data(value, len);
	CSN_high;
}

/* Write an array of bytes into the MiRF register */
void MIRF_write_register(uint8_t reg, uint8_t *value, uint8_t len)
{
	CSN_low;
	SPI_transfer(W_REGISTER | (REGISTER_MASK & reg));
	SPI_write_data(value, len);
	CSN_high;
}

/* Send a data package to the given address. Be sure to send the
 * correct amount of bytes as configured as payload on the receiver.
 * This method also takes care of the RX[0] status and handles the
 * nRF IRQ flags.
 * Returns 1 if data transmission is successful and 0 if not */
uint8_t MIRF_send_data_no_irq(uint8_t *address, uint8_t *data, uint8_t payload_size)
{
	MIRF_flush_tx();

	/* --------------------------------------------------- */
	/* Enable RX pipe 0 if it's not already
	|* I can't think why someone would disable pipe RX[0],
	|* but if so, probably there was a good reason and
	|* it's better to keep that way after the transmission */
	if(!(_mask_pipe & 1)) {
		MIRF_set_register(EN_RXADDR, _mask_pipe | 1);
	}

	/* ------------------------------------ */
	/* Store the address of RX pipe 0
	|* to restore it after the transmission */
	uint8_t previous_address[ADDR_SIZE];
	MIRF_read_register(RX_ADDR_P0, previous_address, ADDR_SIZE);

	/* The RX[0] and TX addresses must be the same because
	|* of the auto acknowldgement mecanism	*/
	MIRF_write_register(TX_ADDR, address, ADDR_SIZE);
	MIRF_write_register(RX_ADDR_P0, address, ADDR_SIZE);

	CSN_low;
	/* Send the command to write the payload */
	SPI_transfer(W_TX_PAYLOAD);
	/* Write the payload */
	SPI_write_data(data, payload_size);
	CSN_high;

	/* Start the transmission */
	MIRF_transmit();

	while(!(MIRF_MAX_RT_REACHED || MIRF_DATA_SENT));

	/* ----------------------------------------------- */
	/* Restore the pipes status. I presume an extra
	|* comparison is better than an unecessary SPI
	|* communication if the pipe 0 was already enabled */
	if(!(_mask_pipe & 2)) {
		MIRF_set_register(EN_RXADDR, _mask_pipe);
	}

	/* Restore the previous RX pipe 0 */
	MIRF_write_register(RX_ADDR_P0, previous_address, ADDR_SIZE);

	if(MIRF_MAX_RT_REACHED) {
		/* Reset MAX_RT */
		MIRF_set_register(STATUS, 1<<MAX_RT);
		return 0;
	} else {
		/* Reset TX_DS */
		MIRF_set_register(STATUS, 1<<TX_DS);
		return 1;
	}
}

/* Send a data package to the given address. Be sure to send the
 * correct amount of bytes as configured as payload on the receiver
 * It's up to the caller to handle the nRF IRQ flags */
void MIRF_send_data(uint8_t *address, uint8_t *data, uint8_t payload_size)
{
	MIRF_flush_tx();

	/* The RX[0] and TX addresses must be the same because
	|* of the auto acknowldgement mecanism	*/
	MIRF_write_register(TX_ADDR, address, ADDR_SIZE);
	MIRF_write_register(RX_ADDR_P0, address, ADDR_SIZE);

	CSN_low;
	/* Send the command to write the payload */
	SPI_transfer(W_TX_PAYLOAD);
	/* Write the payload */
	SPI_write_data(data, payload_size);
	CSN_high;

	/* Start the transmission */
	MIRF_transmit();
}

/* Transmit the TX FIFO content within given delay in
 * microseconds. Enter in Stand-By II mode during this
 * period if the TX FIFO is empty */
void MIRF_transmit()
{
	CE_high;
	_delay_us(15);
	CE_low;
	_delay_us(5);
}
/* Flush the RX and TX FIFOs */
void MIRF_flush_rx_tx(void)
{
	CSN_low;
	SPI_transfer(FLUSH_RX);
	SPI_transfer(FLUSH_TX);
	CSN_high;
}

/* Flush the RX FIFO */
void MIRF_flush_rx(void)
{
	CSN_low;
	SPI_transfer(FLUSH_RX);
	CSN_high;
}

/* Flush the TX FIFO */
void MIRF_flush_tx(void)
{
	CSN_low;
	SPI_transfer(FLUSH_TX);
	CSN_high;
}
/* Wait for data and read it when it arrives */
void MIRF_receive_data(uint8_t *data, uint8_t payload_size)
{
	while(!(MIRF_DATA_READY));
	MIRF_read_data(data, payload_size);
}

/* Read the RX Payload */
void MIRF_read_data(uint8_t *data, uint8_t payload_size)
{
	CE_low;

	CSN_low;
	SPI_transfer(R_RX_PAYLOAD);
	SPI_read_data(data, payload_size);
	CSN_high;
	MIRF_set_register(STATUS, (1<<RX_DR));

	CE_high;
	_delay_us(130);
}

/* Clear the flags for the RX_DR, TX_DS and MAX_RT interrupts */
void MIRF_clear_all_interrupts(void)
{
	MIRF_set_register(STATUS, 1<<MAX_RT);
	MIRF_set_register(STATUS, 1<<RX_DR);
	MIRF_set_register(STATUS, 1<<TX_DS);
}
