#ifndef USART_H
#define USART_H

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

//#define USART_BAUDRATE 9600
//#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

/*
 * SS	PB2
 * MOSI	PB3
 * MISO	PB4
 * SCK	PB5
 * XCK	PD4
 */

class USART{
	public:
		USART(int baud);
		void send(uint8_t);
		uint8_t receive();
};

#endif
