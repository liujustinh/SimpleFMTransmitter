#include <avr/io.h>
#include <util/delay.h>
#define bool int 
#define true 1
#define false 0 

bool left, right, up, down; 


void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	ADMUX |=(1<<REFS0);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}

short readADC(char PIN)
{
	PIN &= 0b00000011; 
	ADMUX = (ADMUX & 0xF8)|PIN;
	for(unsigned short i = 0; i < 15; i++)
	{
		asm("nop");
	} 
	return (ADC);
}

void joys_tick() {
	unsigned short UD, LR;
	UD = readADC(1);
	LR = readADC(0);
	if (UD < 350) {
		down = false;
		right = true; 
		left = false; 
		up = false; 
	}
	else if (UD > 650) {
		right = false;
		down = false; 
		up = false; 
		left = true; 
	}
	else if (LR > 600) {
		left = false;
		right = false; 
		down = false; 
		up = true; 
	}
	else if (LR < 250) {
		up = false;
		down = true; 
		left = false; 
		right = false; 
	}
	else {
		return; 
	}
}

void resetJoystick() {
	left = false; 
	right = false; 
	up = false; 
	down = false; 
}
