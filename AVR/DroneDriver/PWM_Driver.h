#include<avr/io.h>
// Setup for Motor B and C
// Using OC0A and OC0B
// 8-bit Timer/Counter0 w/ PWM
// 3 sources: TOV0, OCF0A, OCF0B
// BOTTOM - 0x00, MAX - 0xFF (255), TOP - 0xFF or OCR0A
// CS02:0 - Clock Select
// TCCR0B - Timer/Counter Control Register
// TCNT0 - timer count 0
// WGM02:0 - Wave Generation Mode (inverted, non-inverted)
// COM0x1:0 - Compare Output Mode and Waveform generation

// Modes of Operation
// Normal Mode - counts to 0xFF then restarts at 0x00
// Clear on Timer Compare Match (CTC) toggle?
// **Fast PWM - WGM02:0 = 3 or 7, bottom->top->bottom...
// 	3 ->  TOP = 0xFF, ** 7-> OCR0A
// Non-inverted - COMnx1:0 = 2
// **Inverted - COMnx1:0 = 3

// Prescalar (N) [1, 8, 256, 1024]

// Fast PWM Governing Equation
// f_pwm = f_clk/(N*(1+OCR0x)) for WGM02 = 7

void setupPWM(void){
	// Set PWM Pins as output
	DDRD |= (1<<PD5) | (1<<PD6) | (1<<PD3);// for motor A, B, and C
	DDRB |= (1<<PB1);// for motor D

	// Setup PWM for Motor B and C (PD5, PD6)
	TCCR0A |= (1<<COM0A1);// | (1<<COM0A0);// Inverted Mode for OC0A (on for that many counts)
	TCCR0A |= (1<<COM0B1);// | (1<<COM0B0);// Inverted Mode for OC0A (on for that many counts)
	TCCR0A |= (1<<WGM00) | (1<<WGM01);// Fast PWM
	TCCR0B |= (1<<CS00);// | (1<<CS02) ;// Set a prescalar [NoClk, 1, 8, 64, 256, 1024]

	// Honestly Not sure how these work..
	OCR0A = 0;// duty cycle (OCR0A/256)
	OCR0B = 0;// duty cycle
	TCNT0 = 0;// Initialize Timer


	// Setup PWM for Motor A (PD3) OC2B
	TCCR2A |= (1<<COM2B1);// | (1<<COM2B0);// Inverted Mode for OCM2B
	TCCR2A |= (1<<WGM01) | (1<<WGM00);// Fast PWM
	TCCR2B |= (1<<CS20);// Set Prescalar to 1
	OCR2B = 0;// duty cycle
	TCNT2 = 0;// Initialize Timer

	// Setup PWM for Motor D (PB1) OC1A
	TCCR1A |= (1<<COM1A1);// Non-inverting mode
	TCCR1A |= (1<<WGM10);// Fast PWM 8-bit
	TCCR1A |= (1<<WGM12);// FaST PWM 8-bit
	TCCR1B |= (1<<CS10);// Set Prescalar to 1
	// Used to set Duty Cycle in Fast PWM Mode
	OCR1AL = 0;// duty cycle lower 8 bits
	OCR1AH = 0;// upper 8-bits (not used)
	TCNT1 = 0;// Initialize Time
}
