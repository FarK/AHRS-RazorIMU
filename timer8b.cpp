#include "timer8b.h"
#include <avr/io.h>

Timer8b::Timer8b(){
	TCCR0A = _BV(COM0A0) | _BV(COM0A1) | ~_BV(COM0B0) | ~_BV(COM0B1) |
		~_BV(WGM01) | ~_BV(WGM00);

	TCCR0B = ~_BV(FOC0A) | ~_BV(FOC0B) | ~_BV(WGM02) | ~_BV(CS02) |
		_BV(CS01) | ~_BV(CS00);
}

uint8_t Timer8b::getDeltaTime(){
	uint8_t tmp = TCNT0 - old_time;
	old_time = TCNT0;
	return tmp;
}
