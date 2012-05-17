#ifndef USART_H
#define USART_H

//#define USART_BAUDRATE 9600
//#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

/*
 * SS	PB2
 * MOSI	PB3
 * MISO	PB4
 * SCK	PB5
 * XCK	PD4
 */

namespace USART_FRAME{
	const uint8_t STX = 0x63;
	const uint8_t DLE = 0x64;
};

class USART{
	public:
		USART(int baud);
		void send(uint8_t);
		void send(uint8_t* buff, uint8_t size);
		void sendFrame(uint8_t* buff, uint8_t size);
		uint8_t receive();
};

#endif
