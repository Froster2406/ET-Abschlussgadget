#define F_CPU (1000000)
#define VALUE1 (33800)
#define VALUE2 (34000)
#define VALUE3 (34400)
#define VALUE4 (34800)
#define VALUE5 (35000)
#define FILTER_ALPHA (1)

#include <avr/io.h>
#include <util/delay.h>
#include "TinyTouchLib.h"
#include "TinyLib.h"

uint8_t irCmd[] = {0x76, 0xB5, 0xAA, 0xB5, 0xAD};
uint16_t bias;
uint16_t prevDelta;

int main(void)
{
	// Set main clock prescaler to 2 (20MHz / 2 = 10MHz)
	CLKCTRL.MCLKCTRLB = 0b00000001;
	
	// Initialize led pins
	led1Init();
	led2Init();
	led3Init();
	led4Init();
	led5Init();
	ledIrInit();
	
	// Set up frequency generator output
	TCA0.SINGLE.CTRLB = 0b00000001;
	// Set top value for 38kHz square wave
	TCA0.SINGLE.CMP0 = 42;
	// Set PTB2 as output
	PORTB.DIRSET = 0b001;
	// Enable tca0
	TCA0.SINGLE.CTRLA = 0b1;
	
	// Initialize touch lib
	tinytouch_init();
	bias = 32000;
	for (uint8_t i = 0; i < 16; i++) {
		bias += tinytouch_adc();
		_delay_ms(100);
	}
	
	while (1) {
		// Average 16 samples
		uint16_t tmp = 0;
		for (uint8_t i = 0; i < 16; i++) {
			tmp += tinytouch_adc();
			_delay_us(100);
		}
			
		// Subtract measured from bias
		uint16_t delta = tmp - bias;
			
		// Low pass filter
		delta = prevDelta * (1 - FILTER_ALPHA) + delta * FILTER_ALPHA;
		
		// Turn on corresponding leds
		ledsOff();
		if (delta > VALUE1) {
			led5On();
		}
		if (delta > VALUE2) {
			led4On();
		}
		if (delta > VALUE3) {
			led3On();
		}
		if (delta > VALUE4) {
			led2On();
		}
		if (delta > VALUE5)	{
			led1On();
			sendIrCommand(irCmd);
		}
			
		prevDelta = delta;
	}
}
