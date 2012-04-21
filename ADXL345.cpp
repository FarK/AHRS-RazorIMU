/* ******************************************************* */
/* I2C code for ADXL345 accelerometer                      */
/*                                                         */
/* ******************************************************* */

//#include <WProgram.h>
#include "ADXL345.h"
#include <Wire.h>
#include <stdint.h>
#include <util/delay.h>
#include "declarations.h"

//I2C addresses 
#define ACCEL_ADDRESS 0x53     //Write:0xA6  Read:0xA7

//Registers addresses
#define POWER_CTL	0X2D	//7:0 6:0 5:Link 4:Auto_Sleep 3:Measure 2:Sleep 1-0:Wakeup
#define DATA_FORMAT	0x31	//7:self_test 6:SPI 5:INT_INVERT 4:0 3:FULL_RES 2:Justify 1-0:Range
#define ACCEL_DATA	0x32	//0x32-0x33: x-axis 0x34-0x35: y-axis 0x36-0x37: z-axis

//Registers values
#define POWER_CTL_VAL	0X08	//Measure on
#define DATA_FORMAT_VAL	0x08	//+-2g range, right justify and full resolution (10bits in +-2g case)

//============================================
// Accelerometer
//============================================
void Init_Accel()
{
  Wire.beginTransmission(ACCEL_ADDRESS);
  Wire.send(POWER_CTL);
  Wire.send(POWER_CTL_VAL);
  //Wire.endTransmission();
  //delay(20);
  //Wire.beginTransmission(ACCEL_ADDRESS);
  Wire.send(DATA_FORMAT);  // Data format register
  Wire.send(DATA_FORMAT_VAL);  // set to full resolution
  Wire.endTransmission();
  _delay_us(20);	
  // Because our main loop runs at 50Hz we adjust the output data rate to 50Hz (25Hz bandwith)
  //Wire.beginTransmission(ACCEL_ADDRESS);
  //Wire.send(0x2C);  // Rate
  //Wire.send(0x09);  // set to 50Hz, normal operation
  //Wire.endTransmission();
}

// Reads x,y and z accelerometer registers
void Read_Accel(s_sensor_data* sen_data, s_sensor_offsets* sen_offset)
{
  int i = 0;
  uint8_t buff[6];
  
  Wire.beginTransmission(ACCEL_ADDRESS); 
  Wire.send(ACCEL_DATA);        //sends address to read from
  //Wire.endTransmission(); //end transmission
  
  //Wire.beginTransmission(ACCEL_ADDRESS); //start transmission to device
  Wire.requestFrom(ACCEL_ADDRESS, 6);    // request 6 bytes from device
  
  while(Wire.available())   // ((Wire.available())&&(i<6))
  { 
    buff[i] = Wire.receive();  // receive one byte
    i++;
  }
  Wire.endTransmission(); //end transmission
  
  if (i==6)  // All bytes received?
    {   
      //this was too messy to do all at once so I broke it up into 3 steps
      
      //get the raw data
      sen_data->axr = ( ((int)buff[1]) << 8) | buff[0]; // X axis
      sen_data->ayr = ( ((int)buff[3]) << 8) | buff[2]; // Y axis 
      sen_data->azr = ( ((int)buff[5]) << 8) | buff[4]; // Z axis
      
      //subtra->t the offset
      sen_data->ax = sen_data->axr - sen_offset->accel_offset[0];
      sen_data->ay = sen_data->ayr - sen_offset->accel_offset[1];
      sen_data->az = sen_data->azr - sen_offset->accel_offset[2];
      
      //change->the sign if needed
      //sen_data->ax *= -1* sensor_sign[3];
      //sen_data->ay *= -1* sensor_sign[4];
      //sen_data->az *= -1* sensor_sign[5];
  
  
    }
  /*else
    Serial.println("!ERR: Error reading accelerometer info!");
  */
}
