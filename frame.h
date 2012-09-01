#include <stdint.h>
#ifndef FRAME_H
#define FRAME_H

//Frame structure to send
//The union allows access as a buffer or to a independent register
typedef union
{
	struct{
		float time;
		float q0;
		float q1;
		float q2;
		float q3;

		float ax;
		float ay;
		float az;

		float gx;
		float gy;
		float gz;

		float mx;
		float my;
		float mz;
	};
	uint8_t buff[14*sizeof(float)];
} Frame;

#endif
