#include "timer8b.h"
#include <avr/io.h>

using namespace Timer8b_Const;

Timer8b::Timer8b(){
	//Timer configuration for normal mode. Initially timer is stopped.
	TCCR0A = (uint8_t) ~_BV(COM0A0) & ~_BV(COM0A1) & ~_BV(COM0B0) & ~_BV(COM0B1) &
		~_BV(WGM01) & ~_BV(WGM00);

	TCCR0B = (uint8_t) ~_BV(FOC0A) & ~_BV(FOC0B) & ~_BV(WGM02) & ~_BV(CS02) &
		~_BV(CS01) & ~_BV(CS00);
}


void Timer8b::start(Prescale prescale, uint8_t initialTime){
	TCNT0 = initialTime;	//Set timer counter
	TCCR0B |= prescale;	//Set clk prescale
}

void Timer8b::pause(){
	//No clk source, timer stopped.
	TCCR0B &= ~_BV(CS02) & ~_BV(CS01) & ~_BV(CS00);
}

void Timer8b::stop(){
	//No clk source, timer stopped.
	TCCR0B &= ~_BV(CS02) & ~_BV(CS01) & ~_BV(CS00);
	TCNT0 = 0;	//Reset timer counter
}

void Timer8b::reset(){
	TCNT0 = 0;	//Reset timer counter
}

