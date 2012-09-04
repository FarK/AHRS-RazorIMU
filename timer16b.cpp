#include "timer16b.h"
#include <avr/io.h>

using namespace Timer16b_Const;

Timer16b::Timer16b(){
	//Timer configuration for normal mode. Initially timer is stopped.
	TCCR1A = (uint8_t) ~_BV(COM1A0) & ~_BV(COM1A1) & ~_BV(COM1B0) &
		           ~_BV(COM1B1) & ~_BV(WGM11) & ~_BV(WGM10);

	TCCR1B = (uint8_t) ~_BV(ICNC1) & ~_BV(ICES1) & ~_BV(WGM13) &
			   ~_BV(WGM12) & ~_BV(CS12) & ~_BV(CS11) & ~_BV(CS00);

	TCCR1C = (uint8_t) ~_BV(FOC1A) & ~_BV(FOC1B);

	TIMSK1 = (uint8_t) _BV(TOIE1);	//Habilita interrupción por overflow
}


void Timer16b::start(Prescale prescale, uint16_t initialTime){
	TCNT1 = initialTime;	//Set timer counter
	TCCR1B |= prescale;	//Set clk prescale
}

void Timer16b::pause(){
	//No clk source, timer stopped.
	TCCR1B &= ~_BV(CS12) & ~_BV(CS11) & ~_BV(CS10);
}

void Timer16b::stop(){
	//No clk source, timer stopped.
	TCCR1B &= ~_BV(CS12) & ~_BV(CS11) & ~_BV(CS10);
	TCNT1 = 0;	//Reset timer counter
}

void Timer16b::reset(){
	TCNT1 = 0;	//Reset timer counter
}

