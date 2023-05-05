// 10MHz
#define F_CPU (10000000)
#define NUMBEROFLEDS (6)
#define VALUE1 (256 / NUMBEROFLEDS)
#define VALUE2 (VALUE1 * 2)
#define VALUE3 (VALUE2 * 3)
#define VALUE4 (VALUE3 * 4)

#include <avr/io.h>
#include <util/delay.h>
#include "tiny_lib.h"
#include "TinyTouchLib.h"

uint8_t irCmd[] = {0x76, 0xB5, 0xAA, 0xB5, 0xAD};

void pcbTest(void)
{
		// turn on all LEDs
		led1On();
		led2On();
		led3On();
		led4On();
		led5On();
		ledsOff();
		ledIrOn();
		_delay_ms(1000);
}

/* output data on PB0 */
void printADCValue(uint8_t value)
{
	PORTB.DIRSET |= 1; /* configure pin as output */
	for(uint8_t i = 0; i < 8; i++){
		if((value >> i) & 1)
		{	/* toggle pin ON / HIGH */
			PORTB.OUTSET |= 1;
		} else 
		{	/* toggle pin OFF / LOW */
			PORTB.OUTCLR |= 1;
		}
		_delay_ms(1);
	}
}

void actualProgram(void)
{
	uint8_t value = 0;
	for(;;)
	{
		value = tinytouch_adc();
		printADCValue(value);
		if (value < VALUE1){
			led1On();
		} else if (value < VALUE2)
		{
			led1On();
			led2On();
		} else if (value < VALUE3)
		{
			led1On();
			led2On();
			led3On();
		} else if (value < VALUE4)
		{
			led1On();
			led2On();
			led3On();
			led4On();
		} else
		{
			led1On();
			led2On();
			led3On();
			led4On();
			led5On();
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
	
	tinytouch_init();
	
	pcbTest();
	actualProgram();
	
}