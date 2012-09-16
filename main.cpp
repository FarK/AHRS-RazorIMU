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
	USART usart;		//Init the USART
	Gyroscope gyr;		//Init the Gyroscope
	Accelerometer acc;	//Init the Accelerometer
	Magnetometer mgt;	//Init the Magnetometer
	_delay_ms(100);
	Timer16b timer;		//Init the Timmer

	timer.start(Timer16b_Const::CLK_8);	//timer start

	uint16_t deltaT = 0;
	float fDeltaT = 0;
	uint16_t deltaT_old = 0;

	//algorithm.calibration(acc, mgt);

	while(1){
		if(mgt.getData(vectMagn)){
			while(!gyr.getData(vectGyro, temperature));
			algorithm.gyroscope(vectGyro, fDeltaT);
			algorithm.magnetometer(vectMagn, fDeltaT);
			algorithm.fusion(fDeltaT);
			algorithm.oarOrientationCorrection();
			algorithm.correction();

			frame.q0 = algorithm.SEq.q0;
			frame.q1 = algorithm.SEq.q1;
			frame.q2 = algorithm.SEq.q2;
			frame.q3 = algorithm.SEq.q3;

			frame.gx = vectGyro.x;
			frame.gy = vectGyro.y;
			frame.gz = vectGyro.z;

			frame.ax = vectAcce.x;
			frame.ay = vectAcce.y;
			frame.az = vectAcce.z;

			frame.mx = vectMagn.x;
			frame.my = vectMagn.y;
			frame.mz = vectMagn.z;

			frame.time += fDeltaT;

			//usart.sendFrame(frame.buff, sizeof(Frame));

			//StatusLEDToggle
			//PORTB ^= _BV(PB5);

			cli();
			deltaT = TCNT1;
			timer.reset();
			sei();
			fDeltaT = (float(deltaT))*1e-6;
		}

		if(!count){
			usart.sendFrame(frame.buff, sizeof(Frame));
			frame.time = 0;
			count = 10;
		}
		--count;
	}
}

//Detección de overflow en el timer
//ISR(TIMER1_OVF_vect){
//	PORTB |= _BV(PB5);
//}
