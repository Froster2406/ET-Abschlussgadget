#ifndef TINY_LIB
#define TINY_LIB

// Set led Ir pin as output
void ledIrInit() {
	PORTB.DIRSET = 0b0100;
}

// Turn led Ir on
void ledIrOn() {
	PORTB.OUTSET = 0b0100;
}

// Turn led Ir off
void ledIrOff() {
	PORTB.OUTCLR = 0b0100;
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
			for (uint8_t j = 0; j < sizeof(irCmd[0]); j++) {
				if (irCmd[i] & (1 << (7 - j))) {
					ledIrOn();
					_delay_us(625);
				}
				else {
					ledIrOff();
					_delay_us(625);
				}
			}
		}
		_delay_ms(25);
	}
}

#endif