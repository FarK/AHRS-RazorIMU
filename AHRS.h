#ifndef AHRS_h
#define AHRS_h
#include "declarations.h"

// quaternion elements representing the estimated orientation
extern float q0, q1, q2, q3;

void AHRSupdate(SensorData* sd, float halfT);

#endif
