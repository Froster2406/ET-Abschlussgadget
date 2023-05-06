// 10MHz
#define F_CPU (1000000)
#define VALUE1 (300)
#define VALUE2 (400)
#define VALUE3 (500)
#define VALUE4 (650)
#define VALUE5 (800)

#include <avr/io.h>
#include <util/delay.h>
#include "tiny_lib.h"
#include "TinyTouchLib.h"

uint8_t irCmd[] = {0x76, 0xB5, 0xAA, 0xB5, 0xAD};
int16_t bias;

void pcbTest(void)
{		
	// turn on all LEDs
	led1On();
	led2On();
	led3On();
	led4On();
	led5On();
	
	// Send ir command
	while(1){
		for(uint8_t n = 0; n < 2; n++) {
			for (uint8_t i = 0; i < sizeof(irCmd)/sizeof(irCmd[0]); i++) {
				for (uint8_t j = 0; j < sizeof(irCmd[0]) * 8; j++) {
					if (irCmd[i] & (1 << (7 - j))) {
						TCA0.SINGLE.CTRLB = 0b01000001;
						_delay_us(2050);
					}
					else {
						TCA0.SINGLE.CTRLB = 0b00000001;
						_delay_us(2050);
					}
				}
			}
			TCA0.SINGLE.CTRLB = 0b00000001;
			_delay_ms(82);
		}
		TCA0.SINGLE.CTRLB = 0b00000001;
		_delay_ms(1000);
	}
	_delay_ms(1000);
}

// output data on PB0 (J1)
void printADCValue(uint8_t value)
{
	// configure pin as output
	PORTB.DIRSET = 0b1;
	for(uint8_t i = 0; i < 8; i++){
		if((value >> i) & 1) {
			// toggle pin ON / HIGH
			PORTB.OUTSET = 0b1;
		}
		else {
			// toggle pin OFF / LOW
			PORTB.OUTCLR = 0b1;
		}
		_delay_ms(1);
	}
}

void actualProgram(void)
{
	for(;;)
	{
		// average 16 samples
		int16_t tmp = 0;
		for (uint8_t i = 0; i < 16; i++) {
			tmp += tinytouch_adc();
			_delay_us(100);
		}

		int32_t delta = bias - tmp;
		
		printADCValue(delta);
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
	}
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
	
	// Set up frequency generator output
	TCA0.SINGLE.CTRLB = 0b00000001;
	// Set top value for 38kHz square wave
	TCA0.SINGLE.CMP0 = 43;
	// Set PTB2 as output
	PORTB.DIRSET = 0b001;
	// Enable tca0
	TCA0.SINGLE.CTRLA = 0b1;
	
	// Initialize touch lib
	tinytouch_init();
	bias = 0;
	for (uint8_t i = 0; i < 16; i++) {
		bias += tinytouch_adc();
		_delay_us(100);
	}
	
	//pcbTest();
	actualProgram();
	
	while (1) {
		
	}
}
