#ifndef TIMER8B_H
#define TIMER8B_H
#include <avr/io.h>

namespace Timer8b_Const{
	typedef enum {CLK_0 = 1, CLK_8, CLK_64, CLK_256, CLK_1024} Prescale;
}

class Timer8b{
	public:
		//Constructor
		Timer8b();

		//Start, stop, pause and reset timer
		void start(Timer8b_Const::Prescale prescale = Timer8b_Const::CLK_8, uint8_t initialTime = 0);
		void pause();
		void stop();
		void reset();

		//Reference to timer register
		//uint8_t& time = TCNT0;
};

#endif
