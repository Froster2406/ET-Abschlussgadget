#ifndef TINY_LIB
#define TINY_LIB

// Always inline these functions
/*static __inline__ void led1On()  __attribute__((__always_inline__));
static __inline__ void led2On()  __attribute__((__always_inline__));
static __inline__ void led3On()  __attribute__((__always_inline__));
static __inline__ void led4On()  __attribute__((__always_inline__));
static __inline__ void led5On()  __attribute__((__always_inline__));
static __inline__ void led6On()  __attribute__((__always_inline__));
static __inline__ void pwm1Init()  __attribute__((__always_inline__));*/

// Turn led 1 on
void led1On() {
	// Pin 1 low, Pin 4 high
	PORTB = (PORTB & ~(1<<PORTB0)) | (1<<PORTB2);
	// Pin 6 input, Pin 1 output, Pin 4 output
	DDRB = (DDRB & ~(1<<DDB3)) | (1<<DDB0) | (1<<DDB2);
}

// Turn led 2 on
void led2On() {
	// Pin 4 low, Pin 6 high
	PORTB = (PORTB & ~(1<<PORTB2)) | (1<<PORTB3);
	// Pin 1 input, Pin 4 output, Pin 6 output
	DDRB = (DDRB & ~(1<<DDB0)) | (1<<DDB2) | (1<<DDB3);
}

// Turn led 3 on
void led3On() {
	// Pin 6 low, Pin 4 high
	PORTB = (PORTB & ~(1<<PORTB3)) | (1<<PORTB2);
	// Pin 1 input, Pin 4 output, Pin 6 output
	DDRB = (DDRB & ~(1<<DDB0)) | (1<<DDB2) | (1<<DDB3);
}

// Turn led 4 on
void led4On() {
	// Pin 4 low, Pin 1 high
	PORTB = (PORTB & ~(1<<PORTB2)) | (1<<PORTB0);
	// Pin 6 input, Pin 1 output, Pin 4 output
	DDRB = (DDRB & ~(1<<DDB3)) | (1<<DDB0) | (1<<DDB2);
}

// Turn led 5 on
void led5On() {
	// Pin 6 low, Pin 1 high
	PORTB = (PORTB & ~(1<<PORTB3)) | (1<<PORTB0);
	// Pin 4 input, Pin 1 output, Pin 6 output
	DDRB = (DDRB & ~(1<<DDB2)) | (1<<DDB0) | (1<<DDB3);
}

// Turn led 6 on
void led6On() {
	// Pin 1 low, Pin 6 high
	PORTB = (PORTB & ~(1<<PORTB0)) | (1<<PORTB3);
	// Pin 4 input, Pin 1 output, Pin 6 output
	DDRB = (DDRB & ~(1<<DDB2)) | (1<<DDB0) | (1<<DDB3);
}

// Initialize pwm on pin 1
void pwm1Init() {
	// PWM mode 8 (PWM, Phase and Frequency Correct) on pin 1
	TCCR0A = (1<<COM0A1) | (1<<WGM00);
	// Prescaler 1
	TCCR0B = (1<<WGM03) | (1<<CS00);
	// Compare value 105 (38,095kHz)
	OCR0A = 105;
}

// Enable pwm output on pin 1
void pwm1On () {
	// Pin 1 output
	DDRB = DDRB | (1<<DDB0);
}

// Disable pwm output on pin 1
void pwm1Off () {
	// Pin 1 input
	DDRB = DDRB & ~(1<<DDB0);
}

#endif
