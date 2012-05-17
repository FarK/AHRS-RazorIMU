#include <avr/io.h>
#include "USART.h"

USART::USART(int baud){
	//Calcule baudrate register value
	long br = ((F_CPU + baud * 8L) / (baud * 16L) - 1);

	//Disable double transmission speed
	UCSR0A &= ~_BV(U2X0);

	//Enable receiver and transmitter
	UCSR0B |= _BV(RXEN0) | _BV(TXEN0);

	//Set frame format to 8 data bits, no parity, 1 stop bit
	UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);

	//Set baud rate. IMPORTANT: The Baud Rate must be set after the transmitter is enabled
	UBRR0 = br;
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
