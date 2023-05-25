#define F_CPU (1000000)

#include <avr/io.h>
#include <util/delay.h>
#include "TinyLib.h"

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
	
	// Turn on all LEDs
	led1On();
	led2On();
	led3On();
	led4On();
	led5On();
	
	// Test ir led
	while(1){
		TCA0.SINGLE.CTRLB = 0b01000001;
		_delay_ms(82);
		TCA0.SINGLE.CTRLB = 0b00000001;
		_delay_ms(82);
	}
}
