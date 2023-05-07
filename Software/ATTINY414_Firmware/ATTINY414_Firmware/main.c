// 10MHz
#define F_CPU (1000000)
#define VALUE1 (1300)
#define VALUE2 (1400)
#define VALUE3 (1500)
#define VALUE4 (1650)
#define VALUE5 (1800)

#include <avr/io.h>
#include <util/delay.h>
#include "tiny_lib.h"
#include "TinyTouchLib.h"

uint8_t irCmd[] = {0x76, 0xB5, 0xAA, 0xB5, 0xAD};
uint16_t bias;

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
void printChar(uint8_t value)
{
	// Configure pin as output
	PORTB.DIRSET = 0b1;
	
	// Start bit
	PORTB.OUTCLR = 0b1;
	_delay_us(319);
	
	for(uint8_t i = 0; i < 8; i++){
		if((value >> i) & 1) {
			// Toggle pin ON / HIGH
			PORTB.OUTSET = 0b1;
		}
		else {
			// Toggle pin OFF / LOW
			PORTB.OUTCLR = 0b1;
		}
		_delay_us(319);
	}
	
	// Stop bit
	PORTB.OUTSET = 0b1;
	_delay_us(319);
	
	// Idle
	PORTB.OUTSET = 0b1;
}

void serialPrint(uint16_t val) {
	char buf[8 * sizeof(uint16_t) + 1];
	char *str = &buf[sizeof(buf) - 1];

	*str = '\0';
	
	if (val < 0) {
		printChar('-');
	}
	
	uint8_t len = 0;
	do {
		char c = val % 10;
		val /= 10;

		*--str = c + '0';
		len++;
	} while(val);
	
	for (uint8_t i = 0; i < len; i++) {
			printChar(str[i]);
	}
	printChar('\n');
}

void actualProgram(void)
{
	for(;;)
	{
		// Average 16 samples
		uint16_t tmp = 0;
		for (uint8_t i = 0; i < 16; i++) {
			tmp += tinytouch_adc();
			_delay_us(100);
		}
		
		// Subtract bias from measured value
		uint16_t delta = bias - tmp;
		serialPrint(delta);
		
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
	bias = 1000;
	for (uint8_t i = 0; i < 16; i++) {
		bias += tinytouch_adc();
		_delay_us(100);
	}
	
	//pcbTest();
	actualProgram();
	
	while (1) {
		
	}
}
