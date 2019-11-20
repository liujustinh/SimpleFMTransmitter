/*
 * matrix_write.h
 *
 * Created: 11/19/2019 4:44:45 PM
 *  Author: Justin
 */ 


#ifndef MATRIX_WRITE_H_
#define MATRIX_WRITE_H_
/*
 * sr_test.c
 *
 * Created: 6/1/2018 11:38:37 PM
 *  Author: Justin
 */ 

#include <avr/io.h>
#include <util/delay.h>

#define blue 1 
#define red 2 
#define column 3 

unsigned char shiftLeft(unsigned char x) {
	return (x << 1) | (x >> (8 - 1));
}

unsigned char shiftRight(unsigned char x) {
	return (x >> 1) | (x << (8 - 1));
}

void Wait()
{
	for(uint8_t i=0;i<30;i++)
	{
		_delay_loop_2(0);
	}
}

void transmit_data(unsigned int port, unsigned char data) {
	int i;
	
	if (port == red) {
		for (i = 0; i < 8 ; ++i) {
			// Sets SRCLR to 1 allowing data to be set
			// Also clears SRCLK in preparation of sending data
			PORTC = 0x08;
			// set SER = next bit of data to be sent.
			PORTC |= SetBit(PORTC, 0, GetBit(data, i));
			// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
			PORTC |= 0x02;
		}
		// set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
		PORTC |= 0x04;
		// clears all lines in preparation of a new transmission
		PORTC &= 0xF0; 
	}
	else if (port == column) {
		for (i = 0; i < 8 ; ++i) {
			// Sets SRCLR to 1 allowing data to be set
			// Also clears SRCLK in preparation of sending data
			PORTC = 0x80;
			// set SER = next bit of data to be sent.
			//PORTC |= ((data >> i) & 0x10);		//0x7F = 0111 1111 
			PORTC = SetBit(PORTC, 4, GetBit(data, i)); 
			// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
			PORTC |= 0x20;
		}
		// set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
		PORTC |= 0x40;
		// clears all lines in preparation of a new transmission
		PORTC &= 0x0F; 
	}
	else if (port == blue) {
		for (i = 0; i < 8 ; ++i) {
			// Sets SRCLR to 1 allowing data to be set
			// Also clears SRCLK in preparation of sending data
			PORTA = 0x80;
			// set SER = next bit of data to be sent.
			//PORTC |= ((data >> i) & 0x10);		//0x7F = 0111 1111
			PORTA = SetBit(PORTA, 4, GetBit(data, i));
			// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
			PORTA |= 0x20;
		}
		// set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
		PORTA |= 0x40;
		// clears all lines in preparation of a new transmission
		PORTA &= 0x0F;
	}
}

void pulseLED() {
	transmit_data(column, 0x00);
	transmit_data(blue, 0xFF);
	transmit_data(red, 0xFF);
}

#endif /* MATRIX_WRITE_H_ */