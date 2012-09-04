#ifndef TIMER16B_H
#define TIMER16B_H
#include <avr/io.h>

namespace Timer16b_Const{
	typedef enum {CLK_0 = 1, CLK_8, CLK_64, CLK_256, CLK_1024} Prescale;
}

class Timer16b{
	public:
		//Constructor
		Timer16b();

		//Start, stop, pause and reset timer
		void start(Timer16b_Const::Prescale prescale = Timer16b_Const::CLK_0, uint16_t initialTime = 0);
		void pause();
		void stop();
		void reset();

		//Reference to timer register
		//uint8_t& time = TCNT0;
};

#endif
