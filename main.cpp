#include <stdint.h>
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
#include "vector.h"

extern "C" void __cxa_pure_virtual(void) {
	while(1);
}

//Structs for storing sensors data
SensorData sen_data = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//Frame to send
Frame frame;

int main()
{
	//Vectores de los sensores
	Vector<int> vectAcce(0,0,0);
	Vector<int> vectMagn(0,0,0);
	Vector<float> vectGyro(0,0,0);
	float temperature = 0;
	
	//Variables for calculate delta time
	uint8_t timeStamp = 0;
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

		gRdy = gyr.refreshData(vectGyro, temperature);
		aRdy = acc.refreshData(vectAcce);
		mRdy = mgt.refreshData(vectMagn);

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

			frame.ax = (float) vectAcce.x;
			frame.ay = (float) vectAcce.y;
			frame.az = (float) vectAcce.z;

			frame.mx = vectMagn.x;
			frame.my = vectMagn.y;
			frame.mz = vectMagn.z;

			frame.gx = vectGyro.x;
			frame.gy = vectGyro.y;
			frame.gz = vectGyro.z;

			frame.time++;

			usart.sendFrame(frame.buff, sizeof(Frame));
		}

		//StatusLEDToggle
		//PORTB ^= _BV(PB5);
	}
}
