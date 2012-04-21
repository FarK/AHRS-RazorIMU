#ifndef HMC55583_H
#define HMC55583_H
#include "declarations.h"

void Init_Compass();
void Read_Compass(s_sensor_data* sen_data, s_sensor_offsets* sen_offset);
void Compass_Heading(s_sensor_data* sen_data);

#endif
