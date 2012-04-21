/* ******************************************************* */
/* I2C code for ITG-3200 Gyro                              */
/*                                                         */
/* ******************************************************* */

#include "ITG3200.h"
#include <WProgram.h>
#include <Wire.h>
#include "declarations.h"

//I2C addresses 
#define GYRO_ADDRESS 0x68      //Write:0xD0  Read:0xD1

// IGT-3200 Sensitivity (from datasheet) => 14.375 LSBs/º/s
// Tested values : 
#define Gyro_Gain 14.375f	//Gyro gain
#define DIVISOR 823.6268f	//Gyro_Gain * (180/PI)

//============================================
// Gyro
//============================================
void Init_Gyro()
{
  //Set the sample rate divider
  Wire.beginTransmission(GYRO_ADDRESS);
  Wire.send(0x15);  // Sample rate divider register
  Wire.send(0);  // Sample rate divider is 1 (register value + 1)
  Wire.endTransmission();
  delay(20);
  
  //Set the DLPF
  Wire.beginTransmission(GYRO_ADDRESS);
  Wire.send(0x16);  // DLPF register
  Wire.send( (0x03<<3) | (0x00<<0) );  // Set the full-scale range to +/- 2000deg/sec, and the low pass filter to 256Hz
  Wire.endTransmission();
  delay(20);	
  
  //Setup the clock reference
  Wire.beginTransmission(GYRO_ADDRESS);
  Wire.send(0x3E);  // Power and clock register
  Wire.send(1);  // Use the PLL with the X Gyro as the clock reference
  Wire.endTransmission();
  delay(20);	
  
  // Because our main loop runs at 50Hz we adjust the output data rate to 50Hz (25Hz bandwith)
  //Wire.beginTransmission(AccelAddress);
  //Wire.send(0x2C);  // Rate
  //Wire.send(0x09);  // set to 50Hz, normal operation
  //Wire.endTransmission();
}

// Reads the angular rates from the Gyro
void Read_Gyro(s_sensor_data* sen_data, s_sensor_offsets* sen_offset)
{
  int i = 0;
  byte buff[8];  //6 bytes of angular rate data, and 2 bytes of temperature data
  
  Wire.beginTransmission(GYRO_ADDRESS); 
  Wire.send(0x1B);        //The temperature and gyro data starts at address 0x1B
  Wire.endTransmission(); //end transmission
  
  Wire.beginTransmission(GYRO_ADDRESS); //start transmission to device
  Wire.requestFrom(GYRO_ADDRESS, 8);    // request 8 bytes from device
  
  while(Wire.available())   // ((Wire.available())&&(i<6))
  { 
    buff[i] = Wire.receive();  // receive one byte
    i++;
  }
  Wire.endTransmission(); //end transmission
  
  if (i==8)  // All bytes received?
    {
      //get the raw data
      //sen_data->gxr = ((((int)buff[2]) << 8) | buff[3]) / DIVISOR;    // X axis 
      //sen_data->gyr = ((((int)buff[4]) << 8) | buff[5]) / DIVISOR;    // Y axis 
      //sen_data->gzr = ((((int)buff[6]) << 8) | buff[7]) / DIVISOR;    // Z axis
      sen_data->gxr = ((int)((buff[2] << 8) | buff[3]));    // X axis 
      sen_data->gyr = ((int)((buff[4] << 8) | buff[5]));    // Y axis 
      sen_data->gzr = ((int)((buff[6] << 8) | buff[7]));    // Z axis
      
      //Restar el offset y pasar a radianes
      sen_data->gx = (sen_data->gxr - sen_offset->gyro_offset[0]) / DIVISOR;    // X axis 
      sen_data->gy = (sen_data->gyr - sen_offset->gyro_offset[1]) / DIVISOR;    // Y axis 
      sen_data->gz = (sen_data->gzr - sen_offset->gyro_offset[2]) / DIVISOR;    // Z axis
      
      //change the sign if needed
      //sen_data->gx *= sensor_sign[0];    // X axis 
      //sen_data->gy *= sensor_sign[1];    // Y axis 
      //sen_data->gz *= sensor_sign[2];    // Z axis
   
    }
  /*else
    Serial.println("!ERR: Error reading Gyro info!");
  */
}
