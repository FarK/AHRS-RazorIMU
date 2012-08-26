#include <util/delay.h>
#include <avr/interrupt.h>
#include "declarations.h"
#include "Wire.h"
#include <math.h>

#include "timer8b.h"
#include "gyroscope.h"
#include "accelerometer.h"
#include "magnetometer.h"
#include "AHRS.h"
#include "USART.h"

extern "C" void __cxa_pure_virtual(void) {
	while(1);
}

//Structs for storing sensors data
SensorData sen_data = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//Frame to send
Frame frame;

int main()
{
	//Variables for calculate delta time
	uint8_t timeStamp = 0;
	uint8_t aDt_old = 0;
	uint8_t aDt= 0;
	uint8_t gDt_old = 0;
	uint8_t gDt= 0;
	uint8_t mDt_old = 0;
	uint8_t mDt= 0;
	uint8_t ahrsDt_old= 0;
	uint8_t ahrsDt= 0;

	//Flags for controlling the sampling of the sensors
	bool aRdy = false;
	bool gRdy = false;
	bool mRdy = false;

	//Configure LED pin for debug
	DDRB |= _BV(PB5);
	PORTB |= _BV(PB5);

	sei();	//Enable interrupts

	Wire.begin();    	//Init the I2C
	_delay_ms(20);
	USART usart(9600);	//Init the USART
	Gyroscope gyr;		//Init the Gyroscope
	Accelerometer acc;	//Init the Gyroscope
	Magnetometer mgt;	//Init the Gyroscope
	Timer8b timer;		//Init the Gyroscope

	timer.start(Timer8b_Const::CLK_64);	//timer start

	while(1){
		if(gyr.dataReady()){
			gyr.getData(&sen_data);
			cli();
			timeStamp = TCNT0;
			gDt = timeStamp - gDt_old;
			gDt_old = timeStamp;
			sei();
			aRdy = true;
		}

		if(acc.dataReady()){
			acc.getData(&sen_data);
			cli();
			timeStamp = TCNT0;
			aDt = timeStamp - aDt_old;
			aDt_old = timeStamp;
			sei();
			gRdy = true;
		}

		if(mgt.dataReady()){
			mgt.getData(&sen_data);
			cli();
			timeStamp = TCNT0;
			mDt = timeStamp - mDt_old;
			mDt_old = timeStamp;
			sei();
			mRdy = true;
		}

		if(aRdy && gRdy && mRdy){
			cli();
			timeStamp = TCNT0;
			ahrsDt = timeStamp - ahrsDt_old;
			ahrsDt_old = timeStamp;
			sei();
			AHRSupdate(&sen_data, ahrsDt*6.25e-6);	//6.25e-6 -> half of Period
			aRdy = false;
			gRdy = false;
			mRdy = false;
		}

		if (TCNT0 > 255/4)
		{
			frame.roll = atan(2*(q0*q1 + q2*q3)/(1-2*(q1*q1 + q2*q2)));
			frame.roll = q0 + q1 + q2 + q3;
			frame.pitch = asin(2*(q0*q2 - q1*q3));
			frame.yaw = atan(2*(q0*q3 + q1*q2)/(1-2*(q2*q2 + q3*q3)));

			frame.time++;

			usart.sendFrame(frame.buff, sizeof(Frame));
		}

		//StatusLEDToggle
		//PORTB ^= _BV(PB5);
	}
}
