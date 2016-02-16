/*
DHT11 Library 0x01

copyright (c) Davide Gironi, 2011

Released under GPLv3.
Please refer to LICENSE file for licensing information.

References:
  - DHT-11 Library, by Charalampos Andrianakis on 18/12/11

*/


#ifndef DHT11_H_
#define DHT11_H_

#include <avr/io.h>

//setup parameters
#define DHT11_DDR DDRA
#define DHT11_PORT PORTA
#define DHT11_PIN PINA
#define DHT11_INPUTPIN PA2

void DHT11_get_data(uint8_t* values);
/* int8_t DHT11_get_temperature(void); */
/* int8_t DHT11_get_humidity(void); */

#endif
