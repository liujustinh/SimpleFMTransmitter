/*
 * spi.h
 *
 * Created: 11/26/2019 7:22:29 AM
 *  Author: Justin
 */ 


#ifndef SPI_H_
#define SPI_H_
#include <avr/interrupt.h>
#include <avr/io.h>

unsigned char receivedData = 0x00; 

char* integer_to_string(int x)
{
	char* buffer = malloc(sizeof(char) * sizeof(int) * 4 + 1);
	if (buffer)
	{
		sprintf(buffer, "%d", x);
	}
	return buffer; // caller is expected to invoke free() on this buffer to release memory
}

void SPI_SlaveInit(void) {
	DDRB = 0x40; PORTB = 0xBF;
	SPCR |= (1<<SPE)|(1<<SPIE);
	SREG |= 0x80;
	//sei();
}

ISR(SPI_STC_vect){
	receivedData = SPDR;
	PORTD |= 0x40;
}




#endif /* SPI_H_ */