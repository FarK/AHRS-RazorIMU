#include "ADXL345.h"
#include <Wire.h>
#include <stdint.h>
#include <util/delay.h>
#include "declarations.h"

//Registers values
#define POWER_CTL_VAL	0X08	//Measure on
#define DATA_FORMAT_VAL	0x08	//+-2g range, right justify and full resolution (10bits in +-2g case)

using namespace ACC_Registers;

Accelerometer::Accelerometer(){
	//Configuring the registers of dispositive
	Wire.beginTransmission(ADDRESS);
	Wire.send(BW_RATE);
	//BW_RATE
	Wire.send(0x09);	//50Hz (100Hz = 0x0A)
	//POWER_CTL
	Wire.send(0x08);	//Measure on
	Wire.endTransmission();
	
	Wire.beginTransmission(ADDRESS);
	Wire.send(DATA_FORMAT);
	Wire.send(0x08);	// set to full resolution
	Wire.endTransmission();

	Wire.beginTransmission(ADDRESS);
	Wire.send(FIFO_CTL);
	Wire.send(0x00);	//FIFO bypassed
	Wire.endTransmission();
}

void Accelerometer::getData(s_sensor_data* sen_data){
	Wire.beginTransmission(ADDRESS); 
	Wire.send(ACCEL_DATA);
	Wire.endTransmission();

	Wire.requestFrom(ADDRESS, 6);    // request 6 bytes from device

	while(Wire.available() < 6);	//We wait 6 bytes

	//get the raw data
	sen_data->axr = Wire.receive() | (Wire.receive() << 8);
	sen_data->ayr = Wire.receive() | (Wire.receive() << 8);
	sen_data->azr = Wire.receive() | (Wire.receive() << 8);

	//convert to float
	sen_data->ax = (float) sen_data->axr;
	sen_data->ay = (float) sen_data->ayr;
	sen_data->az = (float) sen_data->azr;
}

bool Accelerometer::dataReady(){
	Wire.beginTransmission(ADDRESS); 
	Wire.send(INT_SOURCE);
	Wire.endTransmission();

	Wire.requestFrom(ADDRESS, 1);
	while(!Wire.available());

	return (Wire.receive() & 0x80);	//Data_Ready bit enable
}
