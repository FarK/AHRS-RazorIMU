#include "magnetometer.h"
#include "Wire.h"

using namespace MGT_Registers;

Magnetometer::Magnetometer() : 
	xScale(1), yScale(1), zScale(1)
{
	calibrate();

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
	
	//Will start from DATA X MSB and fetch all the others
	Wire.beginTransmission(ADDRESS);
	Wire.send(REG_XH);
	Wire.endTransmission();

}

void Magnetometer::getData(SensorData* sen_data){
	//The pointer to the registers of device always point to REG_XH
	//at this point, so do not need move it. We read the 6 bytes directly
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

void Magnetometer::calibrate(){
	int x, y, z, mx=0, my=0, mz=0, max=0;

	//Setting positive bias configuration requiered for calibration
	Wire.beginTransmission(ADDRESS);
	Wire.send(REG_CONFA);
	Wire.send(0x19);	//Positive bias and 50Hz (not necesary)
	Wire.endTransmission();

	//Will start from DATA X MSB and fetch all the others
	Wire.beginTransmission(ADDRESS);
	Wire.send(REG_XH);
	Wire.endTransmission();
  
	for(int i=10 ; i > 0; --i){ 
		//Wait to data conversion
		while(!dataReady());

		Wire.requestFrom(ADDRESS, 6);
		while(Wire.available() < 6);	//Wait 6 bytes

		x = (Wire.receive() << 8) | Wire.receive();
		y = (Wire.receive() << 8) | Wire.receive();
		z = (Wire.receive() << 8) | Wire.receive();

		//Keep the maximum of each axis
		if (x>mx) mx=x;
		if (y>my) my=y;
		if (z>mz) mz=z;
		
		//Request a new conversion
		Wire.beginTransmission(ADDRESS);
		Wire.send(REG_MODE);
		Wire.send(0x01);	//Single conversion mode
		Wire.endTransmission();
	}

	//Keep the maximum of three axis
	if(mx>max) max=mx;
	if(my>max) max=my;
	if(mz>max) max=mz;
	
	//Calc scales
	xScale = (float) max/mx;
	yScale = (float) max/my;
	zScale = (float) max/mz;
}
