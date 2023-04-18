/*
                                         ATtiny10
                                 +----------------------+
        AIN0/ADC0/OC0A/PCINT0  --| 1 (PB0)  (PB3/RST) 6 |--  ADC3/PCINT3
                                 |                      |
                                 |                      |
                               --| 2 (GND)      (VCC) 5 |--
                                 |                      |
                                 |                      |
 AIN1/ADC1/OC0B/PCINT1/QTouch  --| 3 (PB1)      (PB1) 4 |--  ADC2/PCINT2
                                 +----------------------+

*/


#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include "tiny_lib.h"


int main(void)
{
	/*
	// Blink led on pin 1
	// -----------------------------
	// Pin 1 as output
    DDRB = 0b0001;
    // Toggle OC0A, CTC mode
	TCCR0A = 1<<COM0A0 | 0<<WGM00;
    // CTC mode; use OCR0A
	TCCR0B = 1<<WGM02 | 3<<CS00;
	// 1 second
	OCR0A = 15624;
	*/
	
	/*
	// IR led pwm signal
	// -----------------------------
	led6On();
	pwm1Init();
	pwm1On();
	_delay_us(560);
	pwm1Off();
	_delay_us(560);
	*/
	
	while (1) 
    {
		// Blink 6 charlieplexed leds
		// -----------------------------
		led1On();
		_delay_ms(200);
		led2On();
		_delay_ms(200);
		led3On();
		_delay_ms(200);
		led4On();
		_delay_ms(200);
		led5On();
		_delay_ms(200);
		led6On();
		_delay_ms(200);
    }
}
