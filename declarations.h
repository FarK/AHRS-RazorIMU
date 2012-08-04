#include <stdint.h>
#ifndef DECLARATIONS_H
#define DECLARATIONS_H

//structure for holding raw and calibration corrected data from the sensors
typedef struct
{
	//raw data is uncorrected and corresponds to the
	//true sensor axis, not the redefined platform orientation
	int gxr;
	int gyr;
	int gzr;
	int axr;
	int ayr;
	int azr;
	int mxr;
	int myr;
	int mzr;
	
	//This data has been corrected based on the calibration values
	float gx;
	float gy;
	float gz;
	float ax;
	float ay;
	float az;
	float mx;
	float my;
	float mz;
	
	//Temperature from gyro sensor
	int tmpr;
	float tmp;
	
	//Delta time between samples of each sensor 
	uint8_t aDt;
	uint8_t gDt;
	uint8_t mDt;
}SensorData;

//Frame structure to send
//The union allows access as a buffer or to a independent register
typedef union
{
	struct{
		long time;
		float roll;
		float pitch;
		float yaw;

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
	uint8_t buff[sizeof(long) + 12*sizeof(float)];
} Frame;

#endif
