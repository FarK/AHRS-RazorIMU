#ifndef ITG3200_H
#define ITG3200_H
#include "declarations.h"

void Init_Gyro();
void Read_Gyro(s_sensor_data* sen_data, s_sensor_offsets* sen_offset);

#endif
