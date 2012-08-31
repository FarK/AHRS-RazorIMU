#ifndef USART_H
#define USART_H

#define FRAME_MAX_LENGTH 50

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

	//Recieving state machine constant
	typedef enum {
		WAIT_STX,
		RECIEVING_LENGTH,
		RECIEVING_DATA,
		DISCARD_DLE_L,
		DISCARD_DLE_D
	} State; 
	
	//Variable for recieving state machine
	extern State state;
	extern uint8_t frameLength;
	extern uint8_t receivedData[FRAME_MAX_LENGTH];
	extern uint8_t dataIndx;
};


class USART{
	public:
		//If baud = 0, baudRate = 500000
		USART(int baud = 0);
		void send(uint8_t);
		void send(uint8_t* buff, uint8_t size);
		void sendFrame(uint8_t* buff, uint8_t size);
		uint8_t receive();
};

#endif
