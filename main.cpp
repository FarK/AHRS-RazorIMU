#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Wire.h"

#include "algorithm.h"
#include "timer16b.h"
#include "frame.h"
#include "gyroscope.h"
#include "accelerometer.h"
#include "magnetometer.h"
#include "USART.h"
#include "vector.h"
#include "quaternion.h"

extern "C" void __cxa_pure_virtual(void) {
	while(1);
}

int main()
{
	int count = 50;
	//Frame to send
	Frame frame = {{0, 1,0,0,0, 0,0,0, 0,0,0, 0,0,0}};

	//Vectores de los sensores
	Vector<int> vectAcce(0,0,0);
	Vector<float> vectMagn(0,0,0);
	Vector<float> vectGyro(0,0,0);
	float temperature = 0;
	
	//Configure LED pin for debug
	DDRB |= _BV(PB5);
	PORTB &= ~_BV(PB5);

	sei();	//Enable interrupts

	Wire.begin();    	//Init the I2C
	_delay_ms(20);
	Algorithm algorithm;
	USART usart;	//Init the USART
	Gyroscope gyr;		//Init the Gyroscope
	Accelerometer acc;	//Init the Gyroscope
	PORTB |= _BV(PB5);
	Magnetometer mgt;	//Init the Gyroscope
	PORTB &= ~_BV(PB5);
	_delay_ms(100);
	Timer16b timer;		//Init the Gyroscope

	timer.start(Timer16b_Const::CLK_8);	//timer start

	while(1){
		if(gyr.getData(vectGyro, temperature))
			algorithm.gyroscope(vectGyro, (float(gyr.deltaT))*1e-6);

		if(mgt.getData(vectMagn)){
			algorithm.magnetometer(vectMagn);
			algorithm.fusion();
			algorithm.correction();
		}

		if(!count){
			count = 50;

			frame.q0 = algorithm.ESq.q0;
			frame.q1 = algorithm.ESq.q1;
			frame.q2 = algorithm.ESq.q2;
			frame.q3 = algorithm.ESq.q3;

			frame.ax = algorithm.M.x;
			frame.ay = algorithm.M.y;
			frame.az = algorithm.M.z;

			frame.mx = algorithm.Ms.x;
			frame.my = algorithm.Ms.y;
			frame.mz = algorithm.Ms.z;

			frame.gx = vectGyro.x;
			frame.gy = vectGyro.y;
			frame.gz = vectGyro.z;

			frame.time++;

			usart.sendFrame(frame.buff, sizeof(Frame));

			//StatusLEDToggle
			PORTB ^= _BV(PB5);
		}
		--count;
	}
}
