/*
 * TinyTouchLib.h
 *
 * Created: 09.06.2013 17:25:47			-	v0.1 Initial release (Attiny 10)
 *			10.06.2013					- 	v0.2 ported to ATtiny 25/45/85 and ATtiny13			
 *  Author: Tim (cpldcpu@gmail.com) 
 */ 

#ifndef TINYTOUCHLIB_H_
#define TINYTOUCHLIB_H_
#include <avr/io.h>

enum {
	tt_off = 0,
	tt_on,
	tt_push,
	tt_release,
	tt_timeout
};

//////////////////////////////////////////////////////////////////////////
//
// User definable settings
//
//////////////////////////////////////////////////////////////////////////

// Define pins to use for the reference input and the touch button
// The reference pin is used to charge or discharge the internal
// sample&hold capacitor. This pin is used in output mode and should
// not be shorted to VCC or GND externally.
// The sense pin is connected to the touch-button. To improve noise immunity
// a series resistor can be used.

// The pin number must match the corresponding analog input number ADCx. 
// Default port is PORTA. (ATtiny 414)

#define tt_refpin 2		// Use PA2 as reference pin
#define tt_refadc 2		// Use ADC2 as reference ADC input
#define tt_sensepin 1	// Use PA1 as sense pin
#define tt_senseadc 1	// Use ADC1 as sense ADC input

//////////////////////////////////////////////////////////////////////////
//
// Library functions
//
//////////////////////////////////////////////////////////////////////////

// Library initialization
// Call this once to initialize the library functions and the ADC converter
void	tinytouch_init(void);

// Returns measured value
uint8_t tinytouch_adc(void);

#endif
