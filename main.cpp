// Mongoose Base AHRS firmware v1.1
// Mongoose 9DoF IMU + Barometric pressure sensor
// www.ckdevices.com
//
// Released under Creative Commons License 
// Modifications and additions by Cory Duce to allow it to work with Mongoose hardware
// Based on code by Doug Weibel and Jose Julio which was based on ArduIMU v1.5 by Jordi Munoz and William Premerlani, Jose Julio and Doug Weibel
//

// Axis definition: 
   // X axis pointing forward (to the battery connector)
   // Y axis pointing to the right 
   // Z axis pointing down.
// Positive pitch : nose up
// Positive roll : right wing down
// Positive yaw : clockwise


// The calibration values for the sensors need to be set in the function "mongooseCalibrate" which
// is in the file "ApplicationRoutines.pde"
// The Mongoose Visualizer PC software can be used to determine the offset values by looking at the Avg value 
// for each sensor axis.


/* Mongoose Hardware version - v1.0
	
	ATMega328@3.3V w/ external 8MHz resonator
	High Fuse DA
        Low Fuse FF
	
	ADXL345: Accelerometer
	HMC5843: Magnetometer
	IGT-3200: Gyro
        BMP085: Barometric Pressure sensor
        
        Programmer : 3.3v FTDI
        Arduino IDE : Select board  "Arduino Pro or Pro mini (3.3V, 8MHz) w/ Atmega328"
*/

#include "declarations.h"
#include <WProgram.h>
#include <EEPROM.h>
#include <Wire.h>
#include <math.h>

#include "timing.h"
#include "ADXL345.h"
#include "HMC5843.h"
#include "ITG3200.h"
#include "output.h"
#include "AHRS.h"

extern "C" void __cxa_pure_virtual(void) {
	while(1);
}

s_sensor_offsets sen_offset = {{0,0,0},{0,0,0},{0,0,0},0,0,0,0};
s_sensor_data sen_data = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
euler_angles eAngles;

//These counters allow us to sample some of the sensors at lower rates
unsigned int  Compass_counter=0;
unsigned int  Print_counter=0;

//DCM variables
float G_Dt=0.02;    // Integration time. We will run the integration loop at 50Hz if possible

void setup()
{ 
  Serial.begin(9600);
 
  //pinMode (STATUS_LED,OUTPUT);  // Status LED
  //pinMode (debugPin,OUTPUT);  // debug LED

  delay(30);

  Wire.begin();    //Init the I2C
  delay(20);
  
  //================================
  // Initialize all the IMU sensors
  //
  Init_Accel();
  Init_Gyro();
 

  //===============================
  // Get the calibration value for the sensors. These are hard coded right now
  //mongooseCalibrate(&sen_offset);
  
  
  // All the offset values and calibration factors are now setup for the sensors
  //digitalWrite(STATUS_LED,HIGH);
    

  timer=millis();
  delay(20);
  
  //init some counters
  Compass_counter=0;
  Print_counter=0; 
}

int main()
{
	init();
	setup();

	Magnetometer mgt;

	while(1){
		if((DIYmillis()-timer)>=5)  // Main loop runs at 50Hz
		{
			timer_old = timer;
			timer=DIYmillis();
			G_Dt = (timer-timer_old)/1000.0;    // Real time of loop run. We use this on the DCM algorithm (gyro integration time)
			if(G_Dt > 1)
				G_Dt = 0;  //keeps dt from blowing up, goes to zero to keep gyros from departing

			Compass_counter++;
			Print_counter++;


			//=================================================================================//
			//=======================  Data adquisition of all sensors ========================//


			//======================= Read the Gyro and Accelerometer =======================//
			Read_Gyro(&sen_data, &sen_offset);      // Read the data from the I2C Gyro
			Read_Accel(&sen_data, &sen_offset);     // Read I2C accelerometer


			//=============================== Read the Compass ===============================//
			if(mgt.dataReady()){
				Compass_counter=0;
				mgt.getData(&sen_data);
			}

			//=================================================================================//
			AHRSupdate(&sen_data, G_Dt/2.0);

			//=================================================================================//
			//============================= Data Display/User Code ============================//
			// Make sure you don't take too long here!

			if (Print_counter > 4)
			{
				Print_counter=0;

				//eAngles.roll = atan(2*(q0*q1 + q2*q3)/(1-2*(q1*q1 + q2*q2)));
				eAngles.roll = q0 + q1 + q2 + q3;
				eAngles.pitch = asin(2*(q0*q2 - q1*q3));
				eAngles.yaw = atan(2*(q0*q3 + q1*q2)/(1-2*(q2*q2 + q3*q3)));

				printdata(&sen_data, &eAngles); 
			}
			//StatusLEDToggle();
			//digitalWrite(debugPin,LOW);
		}
	}
}
