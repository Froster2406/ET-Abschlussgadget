/*
 * tinytouchlib.c
 *
 * Created: 09.06.2013 17:25:47			-	v0.1 Initial release (ATtiny 10)
 *			10.06.2013					- 	v0.2 ported to ATtiny 25/45/85 and ATtiny13
 *			02.05.2023					-	v0.3 Ported to ATtiny414
 *  Author: Tim (cpldcpu@gmail.com)
 *  Port by: Froster2406
 */

 
#include "TinyTouchLib.h"
#include <util/delay.h>
#include <stdint.h>

// Internal function to read the adc input
uint8_t tinytouch_adc(void);

uint16_t bias;
uint8_t	touch;
#if touch_timeout>0		
uint8_t timer;
#endif
/*
			Capacitive sensing using charge sharing between 
			the S/H capacitor and an external sensing pad			
*/	

void	tinytouch_init(void) {
	//ADCSRA =_BV(ADEN)|_BV(ADPS2)|_BV(ADPS1); // Enable ADC, Set prescaler to 64
	ADC0.CTRLA = 0b00000100; // enable ADC, disable run standby
	ADC0.CTRLC = 0b01010101; // VDD reference, set prescaler to 64
	bias=tinytouch_adc()<<8;
	touch=0;
}

/* not needed, remove later */
uint8_t tinytouch_sense(void) {
	uint8_t i;
	uint16_t tmp;
	int16_t delta;

	tmp=0;
	for (i=0; i<16; i++) {
		tmp+=tinytouch_adc();	// average 16 samples
		_delay_us(100);
	}

	delta=tmp-(bias>>4);
			
	if (!touch)	{
		if (delta>touch_threshold_on) {
			touch=1;
#if touch_timeout>0
			timer=0;					
#endif
			return tt_push;
		}

		// update bias only when touch not active
		bias=(bias-(bias>>6))+(tmp>>2);		// IIR low pass
		return tt_off;		
	} else {
		if (delta<touch_threshold_off) {
			touch=0;
			return tt_release;
		}

#if touch_timeout>0		
		if (timer==255) {
			bias=tinytouch_adc()<<8;			
			return tt_timeout;
		} 
		timer++;
#endif						
		return tt_on;
	}
}	

uint8_t tinytouch_adc(void) {	
	uint8_t dat1,dat2;
	PORTA.PIN1CTRL = 0x0; // disable inversion, disable pullup, interrupt on pin
	PORTA.PIN2CTRL = 0x0; // disable inversion, disable pullup, interrupt on pin

	// Precharge Low (only attach S&H-capacitor to ADC)
	//ADMUX	=tt_refadc;				// connect S/H cap to reference pin
	ADC0.MUXPOS = tt_refadc;		// connect S/H cap to reference pin
	//PORTB |= _BV(tt_refpin);		// Charge S/H Cap 
	PORTA.OUT |= tt_refpin;			// Charge S/H Cap 
	//PORTB &=~_BV(tt_sensepin);	// Discharge Pad (0)
	PORTA.OUT &= ~(tt_sensepin);	// Discharge Pad (0)
	//DDRB  |= _BV(tt_refpin)|_BV(tt_sensepin);
	PORTA.DIR |= tt_refpin | tt_sensepin;
		
	_delay_us(32);
	
	//DDRB  &=~(_BV(tt_sensepin));	// float pad input, note that pull up is off.
	PORTA.DIR &= ~(tt_sensepin);	// float pad input, note that pull up is off.

	//ADMUX	=tt_senseadc;			// Connect sense input to adc
	ADC0.MUXPOS = tt_senseadc;		// Connect sense input to adc
		
	ADC0.INTCTRL |= 1;				// enables result ready interrupt
	//ADCSRA	|=_BV(ADSC);		// Start conversion	
	ADC0.COMMAND |= 1;				// Start conversion
	//while (!(ADCSRA&_BV(ADIF)));		
	while (!(ADC0.INTFLAGS & ~(ADC0.COMMAND))); // wait until conversion is done
	//ADCSRA	|=_BV(ADIF);		// Clear ADIF
	//dat1=ADCL;
	dat1 = ADC0.RES;				// store result in dat1, clear result ready interrupt bit

	// Precharge High (attach S&H-capacitor as well as pad to ADC)
	//ADMUX  =tt_refadc;			// connect S/H cap to reference pin
	ADC0.MUXPOS  = tt_refadc;		// connect S/H cap to reference pin
	//PORTB &=~_BV(tt_refpin);		// Discharge S/H Cap
	PORTA.DIR &= ~(tt_refpin);		// Discharge S/H Cap
	//PORTB |= _BV(tt_sensepin);	// Charge Pad 
	PORTA.DIR |= tt_sensepin;		// Charge Pad 
	//DDRB  |= _BV(tt_refpin)|_BV(tt_sensepin);
	PORTA.DIR  |= tt_refpin | tt_sensepin;
			
	_delay_us(32);

	//DDRB  &=~(_BV(tt_sensepin));	// float pad input input
	PORTA.DIR &= ~(tt_sensepin);	// float pad input input

	//ADMUX	=tt_senseadc;			// Connect sense input to adc
	ADC0.MUXPOS = tt_senseadc;		// Connect sense input to adc
	
	//ADCSRA	|=_BV(ADSC);		// Start conversion
	ADC0.COMMAND |= 1;				// Start conversion
	//while (!(ADCSRA&_BV(ADIF)));
	while (!(ADC0.INTFLAGS & ~(ADC0.COMMAND))); // wait until conversion is done
	//ADCSRA	|=_BV(ADIF);		// Clear ADIF
	//dat1=ADCL;
	dat2 = ADC0.RES;				// store result in dat2, clear result ready interrupt bit

	return dat2-dat1;
}

