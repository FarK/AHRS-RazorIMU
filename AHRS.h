//=====================================================================================================
// AHRS.h
// S.O.H. Madgwick
// 25th August 2010
//=====================================================================================================
//
// See AHRS.c file for description.
// 
//=====================================================================================================
#ifndef AHRS_h
#define AHRS_h
#include "declarations.h"

//----------------------------------------------------------------------------------------------------
// Variable declaration

extern float q0, q1, q2, q3;	// quaternion elements representing the estimated orientation
extern float q4;

//---------------------------------------------------------------------------------------------------
// Function declaration
void AHRSupdate(s_sensor_data* sd, float halfT);

#endif
//=====================================================================================================
// End of file
//=====================================================================================================
