/* ******************************************************* */
/* I2C head for ADXL345 accelerometer                      */
/*                                                         */
/* ******************************************************* */

#ifndef ADXL345_H
#define ADXL345_H
#include "declarations.h"

void Init_Accel();
// Reads x,y and z accelerometer registers
void Read_Accel(s_sensor_data* sen_data, s_sensor_offsets* sen_offset);

#endif
