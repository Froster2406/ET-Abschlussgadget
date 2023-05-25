#ifndef TINY_LIB
#define TINY_LIB

#define F_CPU (1000000)

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

// Set led Ir pin as output
void ledIrInit() {
	// Set up frequency generator output
	TCA0.SINGLE.CTRLB = 0b00000001;
	// Set top value for 38kHz square wave
	TCA0.SINGLE.CMP0 = 42;
	// Set PTB2 as output
	PORTB.DIRSET = 0b0100;
	// Enable tca0
	TCA0.SINGLE.CTRLA = 0b1;
}

// Turn led Ir on
void ledIrOn() {
	TCA0.SINGLE.CTRLB = 0b01000001;
}

// Turn led Ir off
void ledIrOff() {
	TCA0.SINGLE.CTRLB = 0b00000001;
}

// Set led 1 pin as output
void led1Init() {
	PORTB.DIRSET = 0b1000;
}

// Turn led 1 on
void led1On() {
	PORTB.OUTSET = 0b1000;
}

// Turn led 1 off
void led1Off() {
	PORTB.OUTCLR = 0b1000;
}

// Set led 2 pin as output
void led2Init() {
	PORTA.DIRSET = 0b10000000;
}

// Turn led 2 on
void led2On() {
	PORTA.OUTSET = 0b10000000;
}

// Turn led 2 off
void led2Off() {
	PORTA.OUTCLR = 0b10000000;
}

// Set led 3 pin as output
void led3Init() {
	PORTA.DIRSET = 0b01000000;
}

// Turn led 3 on
void led3On() {
	PORTA.OUTSET = 0b01000000;
}

// Turn led 3 off
void led3Off() {
	PORTA.OUTCLR = 0b01000000;
}

// Set led 4 pin as output
void led4Init() {
	PORTA.DIRSET = 0b00100000;
}

// Turn led 4 on
void led4On() {
	PORTA.OUTSET = 0b00100000;
}

// Turn led 4 off
void led4Off() {
	PORTA.OUTCLR = 0b00100000;
}

// Set led 5 pin as output
void led5Init() {
	PORTA.DIRSET = 0b00010000;
}

// Turn led 5 on
void led5On() {
	PORTA.OUTSET = 0b00010000;
}

// Turn led 5 off
void led5Off() {
	PORTA.OUTCLR = 0b00010000;
}

// Turn all leds off
void ledsOff() {
	PORTA.OUTCLR = 0b11110000;
	PORTB.OUTCLR = 0b1100;
}

// Send command over ir led
void sendIrCommand(uint8_t* irCmd) {
	for(uint8_t n = 0; n < 2; n++) {
		for (uint8_t i = 0; i < sizeof(irCmd)/sizeof(irCmd[0]); i++) {
			for (uint8_t j = 0; j < sizeof(irCmd[0]) * 8; j++) {
				if (irCmd[i] & (1 << (7 - j))) {
					ledIrOn();
					_delay_us(2050);
				}
				else {
					ledIrOff();
					_delay_us(2050);
				}
			}
		}
		ledIrOff();
		_delay_ms(82);
	}
}

// Output char on PB0 (J1)
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

// Output uint16 as ascii chars on PB0 (J1)
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

#endif
