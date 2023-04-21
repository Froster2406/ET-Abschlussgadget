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
#include "TinyTouchLib.h"

uint8_t irCmd[] = {0x76, 0xB5, 0xAA, 0xB5, 0xAD};

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
	
	tinytouch_init();
	
	while (1) 
    {
		/*
		// Capacitive touch sensor
		// -----------------------------
		if (tinytouch_sense() == tt_push) {
			led1On();
			_delay_ms(200);
			ledsOff();
		}
		*/
		
		// Send ir command
		// -----------------------------
		for(uint8_t n = 0; n < 2; n++) {
			for (uint8_t i = 0; i < sizeof(irCmd)/sizeof(irCmd[0]); i++) {
				for (uint8_t j = 0; j < sizeof(irCmd[0]); j++) {
					if (irCmd[i] & (1 << (7 - j))) {
						led6On();
						_delay_us(625);
					}
					else {
						ledsOff();
						_delay_us(625);
					}
				}
			}
			_delay_ms(25);
		}
		
		/*
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
		*/
    }
}
