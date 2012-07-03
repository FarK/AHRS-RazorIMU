#ifndef TIMER8B_H
#define TIMER8B_H
#include <avr/io.h>

class Timer8b{
	public:
		//Constructor
		Timer8b();
		//Returns the time since last call
		uint8_t getDeltaTime();

		//Reference to timer register
		//uint8_t& time = TCNT0;

	private:
		uint8_t old_time;
};

#endif
