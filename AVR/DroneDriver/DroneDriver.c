#ifndef F_CPU
#define F_CPU 1000000UL // Tell controller to use crystal frequency (1Mhz)
#endif
#define BAUDRATE 4800// Because our shitty RC oscilator sucks
#include<util/delay.h>
#include<stdbool.h>
#include "PWM_Driver.h"
#include "BasicUART.h"
#define msgSize 15

char pwmCommand[msgSize] = "not workingyet";// used to store incoming message (as str)
int pwm_A, pwm_B, pwm_C, pwm_D;// 0-255
bool debug = false;

// Parses selected portion of the received String message into an int 
// msg format: s###e  #=0-9, s=start, e=end
int getDutyCycle(char inString[msgSize], int byte){
	int dutyCycle = 100 * ((int) inString[3*byte + 1] - 48) ;// hundreds
	dutyCycle += 10 * ((int) inString[3*byte + 2] - 48);// tens
	dutyCycle += ((int) inString[3*byte + 3] - 48);// ones
	return dutyCycle;
}


int main(void){
	USART_init();// Initialize UART
	setupPWM();// Initialize PWM

	_delay_ms(2000);// wait for UARTs to boot

	pwm_A = 1;
	pwm_B = 1;
	pwm_C = 1;
	pwm_D = 1;

	while(1){// Main Loop
		if(debug == true){
			USART_putstring("DroneDriver: Running!");
			USART_send('\n');
			USART_putstring(pwmCommand);// Echo Command
			USART_send('\n');
		};
		USART_getstring(pwmCommand);// Receive command from UART
		pwm_A = getDutyCycle(pwmCommand, 0);// parse 1st byte
		pwm_B = getDutyCycle(pwmCommand, 1);// parse 2nd byte
		pwm_C = getDutyCycle(pwmCommand, 2);// parse 3rd byte
		pwm_D = getDutyCycle(pwmCommand, 3);// parse 4th byte

		OCR2B = pwm_A;// Set Motor A duty cycle
		OCR0B = pwm_B;// Set Motor B duty cycle
		OCR0A = pwm_C;// Set Motor C duty cycle
		OCR1A = pwm_D;// Set Motor D suty cycle
		_delay_ms(5);

	}
	return 0;
}
