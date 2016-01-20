#include <avrutil/adc.h>

void ADC_init(void)
{
	ADMUX |= 1 << REFS0;
	ADCSRA |= 1 << ADEN;
}

void ADC_setup(uint8_t division_factor, uint8_t trigger_source, uint8_t irq)
{
	ADC_select_prescaler(division_factor);

	if(trigger_source == NORMAL) {
		ADC_disable_trigger_source();
	} else {
		ADC_enable_trigger_source(trigger_source);
	}
	if(irq) {
		ADC_enable_interrupt();
	} else {
		ADC_disable_interrupt();
	}
}

void ADC_enable_interrupt(void)
{
	ADCSRA |= 1 << ADIE;
}

void ADC_disable_interrupt(void)
{
	ADCSRA &= ~(1 << ADIE);
}

void ADC_enable_trigger_source(uint8_t trigger_source)
{
	ADCSRA |= 1 << ADATE;
	ADCSRB |= 7 & trigger_source;
}

void ADC_disable_trigger_source()
{
	ADCSRA &= ~(1 << ADATE);
}

void ADC_select_prescaler(uint8_t division_factor)
{
	ADCSRA |= 7 & division_factor;
}

void ADC_select_channel(uint8_t channel)
{
	ADMUX |= 15 & channel;
}

/*  Use on Normal or Free Running Modes */
void ADC_start_conversion(void)
{
	ADCSRA |=  1 << ADSC;
}

/* Use with interrupt */
void ADC_read(uint8_t *data)
{
	data[0] = ADCL;
	data[1] = ADCH;
}

/* Use without interrupt */
void ADC_wait_and_read(uint8_t *data)
{
	while(!(ADCSRA & (1 << ADIF)));
	ADC_read(data);
	ADCSRA |= 1 << ADIF;
}
