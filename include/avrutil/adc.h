#include <avr/io.h>

#define ADC_FREE_RUNNING		0
#define ADC_ANALOG_COMPARATOR		1
#define ADC_EXT_IRQ0			2
#define ADC_TMR_CNT0_MATCH_A		3
#define ADC_TMR_CNT0_OVERFLOW		4
#define ADC_TMR_CNT1_MATCH_B		5
#define ADC_TMR_CNT1_OVERFLOW		6
#define ADC_TMR_CNT1_CAPTURE_EVENT	7
#define ADC_NORMAL			8

void ADC_init();

void ADC_setup(uint8_t division_factor, uint8_t trigger_source, uint8_t irq);

void ADC_enable_interrupt(void);

void ADC_disable_interrupt(void);

void ADC_enable_trigger_source(uint8_t trigger_source);

void ADC_disable_trigger_source(void);

void ADC_select_prescaler(uint8_t division_factor);

void ADC_select_channel(uint8_t channel);

/*  Use on Normal or Free Running Modes */
void ADC_start_conversion();

/* Use with interrupt */
void ADC_read(uint8_t *data);

/* Use without interrupt */
void ADC_wait_and_read(uint8_t *data);
