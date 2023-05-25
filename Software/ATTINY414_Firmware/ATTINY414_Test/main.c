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
	
	// Turn on all LEDs
	led1On();
	led2On();
	led3On();
	led4On();
	led5On();
	
	// Test ir led
	while(1){
		ledIrOn();
		_delay_ms(82);
		ledIrOff();
		_delay_ms(82);
	}
}
