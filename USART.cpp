#include <avr/io.h>
#include "USART.h"

using namespace USART_FRAME;

USART::USART(int baud){
	//Calcule baudrate register value
	long br = ((F_CPU + baud * 8L) / (baud * 16L) - 1);

	//Disable double transmission speed
	UCSR0A &= ~_BV(U2X0);

	//Enable receiver and transmitter
	UCSR0B |= _BV(RXEN0) | _BV(TXEN0);
	//Enable rx interrup too
	//UCSR0B |= _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);

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

void USART::send(uint8_t* buffer, uint8_t size){
	while(size--)
		send(*(buffer++));
}

void USART::sendFrame(uint8_t* buffer, uint8_t size){
	if(size){
		send(STX);
		while(size--){
			if(*buffer == DLE || *buffer == STX) send(DLE);
			send(*buffer++);
		}
	}
}

uint8_t USART::receive(){
	// Wait for byte to be received
	while(!(UCSR0A & (1<<RXC0)));
	// Return received data
	return UDR0;
}

/* Rx interrupt subrutine
IST(UART0_RX_vect){
	//CODE HERE
}
*/
