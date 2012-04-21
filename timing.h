#ifndef TIMING_H
#define TIMING_H

extern unsigned long timer0_millis;
extern long timer;   //general purpuse timer
extern long timer_old;
extern long timer24; //Second timer used to print values 

unsigned long DIYmillis();
void DIYdelay(unsigned long ms);

#endif
