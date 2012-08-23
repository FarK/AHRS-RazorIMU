#include "ADXL345.h"
#include <stdint.h>
#include <avr/interrupt.h>
#include "Wire.h"
#include "vector.h"

//Registers values
#define POWER_CTL_VAL	0X08	//Measure on
#define DATA_FORMAT_VAL	0x08	//+-2g range, right justify and full resolution (10bits in +-2g case)

using namespace ACC_Registers;

Accelerometer::Accelerometer() : deltaT(0), deltaT_old(0){
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

bool Accelerometer::refreshData(Vector<int> &vector){
	if(dataReady()){
		Wire.beginTransmission(ADDRESS); 
		Wire.send(ACCEL_DATA);
		Wire.endTransmission();

		Wire.requestFrom(ADDRESS, 6);    // request 6 bytes from device

		while(Wire.available() < 6);	//We wait 6 bytes

		//get the raw data
		vector.x = Wire.receive() | (Wire.receive() << 8);
		vector.y = Wire.receive() | (Wire.receive() << 8);
		vector.z = Wire.receive() | (Wire.receive() << 8);

		//Actulize deltaT
		cli();
		uint8_t timeStamp = TCNT0;
		deltaT = timeStamp - deltaT_old;
		deltaT_old = timeStamp;
		sei();
		
		return true;
	}
	else return false;
}

bool Accelerometer::dataReady(){
	Wire.beginTransmission(ADDRESS); 
	Wire.send(INT_SOURCE);
	Wire.endTransmission();

	Wire.requestFrom(ADDRESS, 1);
	while(!Wire.available());

	return (Wire.receive() & 0x80);	//Data_Ready bit enable
}
