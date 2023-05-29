#define F_CPU (1000000)
#define VALUE1 (33800)
#define VALUE2 (34500)
#define VALUE3 (35400)
#define VALUE4 (35700)
#define VALUE5 (35900)

#include <avr/io.h>
#include <util/delay.h>
#include "TinyTouchLib.h"
#include "TinyLib.h"

uint8_t irCmd[] = {0x76, 0xB5, 0xAA, 0xB5, 0xAD};

uint16_t getTouchValue() {
	// Average 16 samples
	uint16_t tmp = 0;
	for (uint8_t i = 0; i < 16; i++) {
		tmp += tinytouch_adc();
		_delay_us(100);
	}
	
	return tmp - 32000;
}

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
	
	// Initialize touch lib
	tinytouch_init();
	
	// Startup animation
	for (int8_t i = 0; i < 42; i++) {
		for (int8_t j = 0; j < 50; j++) {
			led1On();
			led2On();
			led3On();
			led4On();
			led5On();
			for (int8_t k = 0; k < i; k++) {
				_delay_us(1);
			}
			led1Off();
			led2Off();
			led3Off();
			led4Off();
			led5Off();
			for (int8_t k = i; k < 42; k++) {
				_delay_us(1);
			}
		}
	}
	for (int8_t i = 0; i < 42; i++) {
		for (int8_t j = 0; j < 50; j++) {
			led1On();
			led2On();
			led3On();
			led4On();
			led5On();
			for (int8_t k = i; k < 42; k++) {
				_delay_us(1);
			}
			led1Off();
			led2Off();
			led3Off();
			led4Off();
			led5Off();
			for (int8_t k = 0; k < i; k++) {
				_delay_us(1);
			}
		}
	}
	
	while (1) {
		uint16_t measured = getTouchValue();
		
		// Check if finger touches pad
		if (measured> VALUE1) {
			// Measuring animation
			ledsOff();
			for (int8_t i = 0; i < 2; i++) {
				led1On();
				_delay_ms(100);
				led1Off();
				led2On();
				_delay_ms(100);
				led2Off();
				led3On();
				_delay_ms(100);
				led3Off();
				led4On();
				_delay_ms(100);
				led4Off();
				led5On();
				
				// Cancel measurement if finger is removed
				measured = getTouchValue();
				if (measured < VALUE1) {
					ledsOff();
					break;
				}
				
				_delay_ms(100);
				led5Off();
				led4On();
				_delay_ms(100);
				led4Off();
				led3On();
				_delay_ms(100);
				led3Off();
				led2On();
				_delay_ms(100);
				led2Off();
				
				// Cancel measurement if finger is removed
				measured = getTouchValue();
				if (measured < VALUE1) {
					ledsOff();
					break;
				}
			}
			
			measured = getTouchValue();
			
			// Display result
			ledsOff();
			if (measured > VALUE1) {
				led5On();
			}
			if (measured > VALUE2) {
				led4On();
			}
			if (measured > VALUE3) {
				led3On();
			}
			if (measured > VALUE4) {
				led2On();
			}
			if (measured > VALUE5)	{
				led1On();
				sendIrCommand(irCmd);
			}
			
			// Wait until finger is removed from pad
			while (measured > VALUE1) {
				measured = getTouchValue();
			}
			
			ledsOff();
		}
	}
}
