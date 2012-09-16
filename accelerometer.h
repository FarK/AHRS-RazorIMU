#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <stdint.h>
#include "vector.h"

namespace ACC_Registers{
	//I2C addresses 
	const uint8_t ADDRESS =	0x53;	//I2C address

	//Registers addresses
	const uint8_t BW_RATE = 	0x2C;	//7-5:0 4:Low Power 3-0: Rate
	const uint8_t POWER_CTL =	0X2D;	//7:0 6:0 5:Link 4:Auto_Sleep 3:Measure 2:Sleep 1-0:Wakeup
	const uint8_t INT_SOURCE = 	0x30;	//7:Data Ready 6:Single Tap 5:Double Tap 4:Activity 3:Inactivity 2:Free Fall 1:Watermark 0:Overrun
	const uint8_t DATA_FORMAT =	0x31;	//7:self_test 6:SPI 5:INT_INVERT 4:0 3:FULL_RES 2:Justify 1-0:Range
	const uint8_t ACCEL_DATA =	0x32;	//0x32-0x33: x-axis 0x34-0x35: y-axis 0x36-0x37: z-axis
	const uint8_t FIFO_CTL = 	0X38;	//7-6:Mode 5:Trigger 4-0:Samples
	const uint8_t FIFO_STATUS =	0X39;	//7:Triger 6:0 5-0:Entries
}


class Accelerometer{
	public:
		Accelerometer();
		bool getData(Vector<int> &vector);

		uint16_t deltaT;
		uint16_t deltaT_old;

	private:
		bool dataReady();
};

#endif
