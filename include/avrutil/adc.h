#include <avr/io.h>

void ADC_init(uint8_t division_factor);

void ADC_enable_interrupt(void);

void ADC_select_channel(uint8_t channel);

void ADC_start_conversion();

void ADC_read_data(uint8_t *data);
