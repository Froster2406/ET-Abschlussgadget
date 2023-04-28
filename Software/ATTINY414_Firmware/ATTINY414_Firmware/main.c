// 10MHz
#define F_CPU 10000000

#include <avr/io.h>
#include <util/delay.h>
#include "tiny_lib.h"
#include "TinyTouchLib.h"

uint8_t irCmd[] = {0x76, 0xB5, 0xAA, 0xB5, 0xAD};

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
	
    while (1) 
    {
		#define DEL 100
		
		// Blink 5 leds
		led1On();
		_delay_ms(DEL);
		led2On();
		_delay_ms(DEL);
		led3On();
		_delay_ms(DEL);
		led4On();
		_delay_ms(DEL);
		led5On();
		_delay_ms(DEL);
		ledsOff();
		_delay_ms(DEL);
		
		// Send command on ir led
		sendIrCommand(irCmd);
    }
}
