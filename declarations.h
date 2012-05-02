#include <stdint.h>
#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#define ToRad(x) (x*0.01745329252)  // *pi/180
#define ToDeg(x) (x*57.2957795131)  // *180/pi

#define FALSE 0
#define TRUE 1

// ADXL345 Sensitivity(from datasheet) => 4mg/LSB   1G => 1000mg/4mg = 256 steps
// Tested value : 256
#define GRAVITY 256  //this equivalent to 1G in the raw data coming from the accelerometer 
#define Accel_Scale(x) x*(GRAVITY/9.81)//Scaling the raw data of the accel to actual acceleration in meters for seconds square

//extern int sensor_sign[9];  //Correct directions x,y,z - gyros, accels, magnetormeter
#define Kp_ROLLPITCH 0.0125
#define Ki_ROLLPITCH 0.000008
#define Kp_YAW 1.2
#define Ki_YAW 0.000008



//========================================
// Output Data Configuration
// Turn on/off the different data outputs
// For the Mongoose Visualization Software to work, the first 3 need to be turned on
// To achieve the max sample rate, you will need to only turn on PRINT_EULER

#define SEND_TRAMA		1   //Envía la trama
#define PRINT_QUATERNION	0   //Imprime el quaternion
#define PRINT_EULER             0   //Will print the Euler angles Roll, Pitch and Yaw
#define PRINT_SENSOR_DATA       0   //Will print the Corrected Sensor Data
#define PRINT_SENSOR_DATA_RAW   0   //Will print the raw uncorrected Sensor Data
#define PRINT_DCM               0   //Will print the whole direction cosine matrix
#define PRINT_MagCal            0

//#define PRINT_GPS 0     //Will print GPS data
//#define PRINT_BINARY 0  //Will print binary message and suppress ASCII messages (above)

#define debugPin 6
#define STATUS_LED 4  //PD4 on the Atmega328. Red LED

//EEPROM storage addesses
#define Start_SensorOffsets    0        //offset data at start of EEPROM
#define Size_SensorOffsets     36       //the offset data is 12 bytes long 

//extern float AN[9]; //array that store the 3 ADC filtered data

//extern float AN_OFFSET[9] = {0,0,0,0,0,0,0,0,0}; //Array that stores the Offset of the sensors

//Structure for holding offsets and calibration values for the accel, gyro, and magnetom
typedef struct
{
    float gyro_offset[3];
    float accel_offset[3];
    float magnetom_offset[3];
    float magnetom_XY_Theta;
    float magnetom_XY_Scale;
    float magnetom_YZ_Theta;
    float magnetom_YZ_Scale;
    
}s_sensor_offsets;


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
}s_sensor_data;

// Euler angles
typedef struct
{
	float roll;
	float pitch;
	float yaw;
}euler_angles;

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
} frame;

#endif
