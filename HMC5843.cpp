#include "HMC5843.h"
#include <Wire.h>

using namespace MGT_Registers;

Magnetometer::Magnetometer() : 
	xScale(1), yScale(1), zScale(1)
{
	//calibrate();

	//Configuring device
	//Will start from DATA X MSB and fetch all the others
	Wire.beginTransmission(ADDRESS);

	Wire.send(REG_CONFA);
	//REG_CONFA
	Wire.send(0x18);	//50Hz & Normal measurement mode
	//REG_CONFB
	Wire.send(0x20);	//Gain = 1; 1300LSb/mGauss
	//REG_MODE
	Wire.send(0x01);	//Single conversion mode

	Wire.endTransmission();
}

void Magnetometer::getData(s_sensor_data* sen_data){
	//Will start from DATA X MSB and fetch all the others
	Wire.beginTransmission(ADDRESS);
	Wire.send(REG_XH);
	Wire.endTransmission();

	Wire.requestFrom(ADDRESS, 6);
	while(Wire.available() < 6);	//Wait 6 bytes
	//Read out the 3 values, 2 bytes each.
	sen_data->mxr = (Wire.receive() << 8) | Wire.receive();
	sen_data->myr = (Wire.receive() << 8) | Wire.receive();
	sen_data->mzr = (Wire.receive() << 8) | Wire.receive();

	//Set scaled values
	sen_data->mx = (float) sen_data->mxr / xScale;
	sen_data->my = (float) sen_data->myr / yScale;
	sen_data->mz = (float) sen_data->mzr / zScale;

	//Request a new conversion
	Wire.beginTransmission(ADDRESS);
	Wire.send(REG_MODE);
	Wire.send(0x01);	//Single conversion mode
	Wire.endTransmission();
}

bool Magnetometer::dataReady(){
	Wire.beginTransmission(ADDRESS);
	Wire.send(REG_STATUS);
	Wire.endTransmission();

	Wire.requestFrom(ADDRESS, 1);
	while(!Wire.available());

	return (Wire.receive() & 0x01);	//RDY bit is set
}
