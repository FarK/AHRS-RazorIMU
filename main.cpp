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
	int count = 10;
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
	USART usart;		//Init the USART
	Gyroscope gyr;		//Init the Gyroscope
	Accelerometer acc;	//Init the Accelerometer
	Magnetometer mgt;	//Init the Magnetometer
	_delay_ms(100);
	Timer16b timer;		//Init the Timmer

	timer.start(Timer16b_Const::CLK_8);	//timer start

	//algorithm.calibration(acc, mgt);

	while(1){
		if(gyr.getData(vectGyro, temperature))
			algorithm.gyroscope(vectGyro, (float(gyr.deltaT))*1e-6);

		if(mgt.getData(vectMagn)){
			algorithm.magnetometer(vectMagn);
			algorithm.fusion();
			algorithm.oarOrientationCorrection();
			algorithm.correction();

			if(!count){
				count = 5;

				frame.q0 = algorithm.ESq.q0;
				frame.q1 = algorithm.ESq.q1;
				frame.q2 = algorithm.ESq.q2;
				frame.q3 = algorithm.ESq.q3;

				frame.gx = vectGyro.x;
				frame.gy = vectGyro.y;
				frame.gz = vectGyro.z;

				frame.ax = vectAcce.x;
				frame.ay = vectAcce.y;
				frame.az = vectAcce.z;

				frame.mx = vectMagn.x;
				frame.my = vectMagn.y;
				frame.mz = vectMagn.z;

				++frame.time;

				usart.sendFrame(frame.buff, sizeof(Frame));

				//StatusLEDToggle
				PORTB ^= _BV(PB5);
			}
			--count;
		}
	}
}

//Detección de overflow en el timer
//ISR(TIMER1_OVF_vect){
//	PORTB |= _BV(PB5);
//}
