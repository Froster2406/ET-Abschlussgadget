/*
 * tinytouchlib.c
 *
 * Created: 09.06.2013 17:25:47			-	v0.1 Initial release (ATtiny 10)
 *			10.06.2013					- 	v0.2 ported to ATtiny 25/45/85 and ATtiny13
 *			02.05.2023					-	v0.3 Ported to ATtiny414
 *  Author: Tim (cpldcpu@gmail.com)
 *  Port by: Froster2406
 */

#define F_CPU (1000000)

#include "TinyTouchLib.h"
#include <util/delay.h>
#include <stdint.h>

/*
			Capacitive sensing using charge sharing between 
			the S/H capacitor and an external sensing pad			
*/	

void	tinytouch_init(void) {
	// Enable ADC, 8-Bit resolution
	ADC0.CTRLA = 0b00000101;
	// VDD reference, set prescaler to 64
	ADC0.CTRLC = 0b00010101;
}

uint8_t tinytouch_adc(void) {	
	uint8_t dat1,dat2;
	
	PORTA.DIRCLR = 1 << tt_refpin;
	
	// Precharge Low (only attach S&H-capacitor to ADC)
	// Connect S/H cap to empty pin (PTB1)
	ADC0.MUXPOS = 10;
	// Charge S/H Cap 
	PORTA.OUTSET = 1 << tt_refpin;
	PORTA.DIRSET = 1 << tt_refpin;
	// Discharge Pad
	PORTA.OUTCLR = 1 << tt_sensepin;
	PORTA.DIRSET = 1 << tt_sensepin;
	
	_delay_us(100);

	// float pad input
	PORTA.DIRCLR = 1 << tt_sensepin;
	// Connect sense input to adc
	ADC0.MUXPOS = tt_senseadc;
	// Start conversion
	ADC0.COMMAND |= 1;
	
	// Wait until conversion is done
	while (ADC0.COMMAND & 0b1) {}
	// store result
	dat1 = ADC0.RES;
	
	// Precharge High (attach S&H-capacitor as well as pad to ADC)
	// connect S/H cap to reference pin
	ADC0.MUXPOS  = tt_refadc;
	// Discharge S/H Cap
	PORTA.OUTCLR = 1 << tt_refpin;
	// Charge Pad
	PORTA.OUTSET = 1 << tt_sensepin;
	PORTA.DIRSET = 1 << tt_sensepin;
	
	_delay_us(100);

	// float pad input
	PORTA.DIRCLR = 1 << tt_sensepin;
	// Connect sense input to adc
	ADC0.MUXPOS = tt_senseadc;
	// Start conversion
	ADC0.COMMAND |= 1;
	
	// wait until conversion is done
	while (ADC0.COMMAND & 0b1) {}
	// store result in dat2
	dat2 = ADC0.RES;
	
	// Return difference
	return dat2-dat1;
}
