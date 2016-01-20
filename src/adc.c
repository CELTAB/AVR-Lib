#include <avrutil/adc.h>

void ADC_init(uint8_t division_factor)
{
	ADMUX |= 1 << REFS0;
	ADCSRA |= (1 << ADEN) | (7 & division_factor);
}

void ADC_enable_interrupt(void)
{
	ADCSRA |= 1 << ADIE;
}

void ADC_select_channel(uint8_t channel)
{
	ADMUX |= 15 & channel;
}

void ADC_start_conversion(void)
{
	ADCSRA |=  1 << ADSC;
}

void ADC_read_data(uint8_t *data)
{
	data[0] = ADCL;
	data[1] = ADCH;
}
