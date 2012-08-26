#include "gyroscope.h"

#include <stdint.h>
#include <avr/interrupt.h>
#include "Wire.h"
#include "vector.h"

using namespace GYR_Registers;

Gyroscope::Gyroscope() : deltaT(0), deltaT_old(0){
	//Set the sample rate divider to 1
	Wire.beginTransmission(ADDRESS);
	Wire.send(SMPLRT_DIV);
	Wire.send(0x00);  // Sample rate divider is 1 (register value + 1)
	Wire.endTransmission();
	
	//Set the Digital Low Pass Filter and Scale Factor
	Wire.beginTransmission(ADDRESS);
	Wire.send(DLPF_FS);
	Wire.send(0x18);  //Scale range = +/-2000deg/sec, and low pass filter = 256Hz
	Wire.endTransmission();
	
	//Setup the interruptions
	Wire.beginTransmission(ADDRESS);
	Wire.send(INT_CFG);
	Wire.send(0x05);	//Device-ready and data-ready interruptions actived
	Wire.endTransmission();
	
	//Setup the clock reference and power status
	Wire.beginTransmission(ADDRESS);
	Wire.send(PWR_MGM);
	Wire.send(0x01);  // Use the PLL with the X Gyro as the clock reference
	Wire.endTransmission();

	//TODO: No funciona: Bucle infinito
	//Wait for device ready
	/*
	int ready = 0;
	while(!ready){
		Wire.beginTransmission(ADDRESS);
		Wire.send(INT_STATUS);
		Wire.endTransmission();

		Wire.requestFrom(ADDRESS, 1);
		while(!Wire.available());
		ready = (Wire.receive() & 0x04); //ING_RDY bit set
	}
	*/
}

bool Gyroscope::getData(Vector<float> &vector, float &temperature){
	if(dataReady()){
		Wire.beginTransmission(ADDRESS); 
		Wire.send(DATA);	//The temperature and gyro data start
		Wire.endTransmission();
		
		Wire.requestFrom(ADDRESS, 8);    // request 8 bytes from device
		
		//Wait to 8 bits
		while(Wire.available() < 8);
		
		//Save data
		int tmpr = (Wire.receive() << 8) | Wire.receive();
		int gxr = (Wire.receive() << 8) | Wire.receive();
		int gyr = (Wire.receive() << 8) | Wire.receive();
		int gzr = (Wire.receive() << 8) | Wire.receive();

		//TODO: La conversión a grados centígrados está mal
		//Convert to degrees centigrade
		temperature = (float) ((tmpr + TEMP_OFFSET) / TEMP_SCALE) + 35.0f;
		//Convert to rad/s
		vector.x = gxr / GYRO_SCALE;
		vector.y = gyr / GYRO_SCALE;
		vector.z = gzr / GYRO_SCALE;

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

bool Gyroscope::dataReady(){
	Wire.beginTransmission(ADDRESS); 
	Wire.send(INT_STATUS);
	Wire.endTransmission();

	Wire.requestFrom(ADDRESS, 1);
	while(!Wire.available());
	return (Wire.receive() & 0x01);	//RAW_DATA_RDY bit set
}
