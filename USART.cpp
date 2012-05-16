#include <avr/io.h>
//#include <avr/sfr_defs.h>
#include "USART.h"

USART::USART(uint8_t baud){
	/* Setting the XCKn port pin as output, enables master mode. */
	//DDRB = _BV(PB2) | _BV(PB3) | ~(_BV(PB4)) | _BV(PB5);
	//DDRD |= _BV(PD4);
	/* Set MSPI mode of operation and SPI data mode 0. */
	//UCSR0C = (1<<UMSEL1)|(1<<UMSEL0)|(0<<UCPHA)|(0<<UCPOL);
	/* Enable receiver and transmitter. */
	UCSR0B |= _BV(RXEN0) | _BV(TXEN0);
	//Set frame format to 8 data bits, no parity, 1 stop bit
	UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
	/* Set baud rate. */
	/* IMPORTANT: The Baud Rate must be set after the transmitter is enabled
	 * */
	#include <util/setbaud.h>
	UBRR0 = UBRR_VALUE;
}

void USART::send(uint8_t byte){
	//wait while previous byte is completed
	while(!(UCSR0A & (1<<UDRE0)));
	// Transmit data
	UDR0 = byte;
}

uint8_t USART::receive(){
	// Wait for byte to be received
	while(!(UCSR0A & (1<<RXC0)));
	// Return received data
	return UDR0;
}
