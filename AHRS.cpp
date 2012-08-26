#include "AHRS.h"
#include "declarations.h"
#include <math.h>
#include "mathUtils.h"

#define Kp 2.0f			// proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.005f		// integral gain governs rate of convergence of gyroscope biases
//#define halfT 0.01f		// half the sample period

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;	// quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0;	// scaled integral error

float invSqrt(float number);	//Optimize inverse square root function

void AHRSupdate(SensorData* sd, float halfT) {
	float inorm;
	float ax, ay, az, gx, gy, gz, mx, my, mz;
	float hx, hy, hz, bx, bz;
	float vx, vy, vz, wx, wy, wz;
	float ex, ey, ez;

	// auxiliary variables to reduce number of repeated operations
	float q0q0 = q0*q0;
	float q0q1 = q0*q1;
	float q0q2 = q0*q2;
	float q0q3 = q0*q3;
	float q1q1 = q1*q1;
	float q1q2 = q1*q2;
	float q1q3 = q1*q3;
	float q2q2 = q2*q2;   
	float q2q3 = q2*q3;
	float q3q3 = q3*q3;          
	
	// normalise the measurements
	inorm = sqrt(sd->ax*sd->ax + sd->ay*sd->ay + sd->az*sd->az);       
	if(inorm){
		ax = sd->ax / inorm;
		ay = sd->ay / inorm;
		az = sd->az / inorm;
	}else{
		ax = 0;
		ay = 0;
		az = 0;
	}

	inorm = sqrt(sd->mx*sd->mx + sd->my*sd->my + sd->mz*sd->mz);       
	if(inorm){
		mx = sd->mx / inorm;
		my = sd->my / inorm;
		mz = sd->mz / inorm;         
	}else{
		mx = 0;
		my = 0;
		mz = 0;         
	}
	
	// compute reference direction of flux
	hx = 2*mx*(0.5 - q2q2 - q3q3) + 2*my*(q1q2 - q0q3) + 2*mz*(q1q3 + q0q2);
	hy = 2*mx*(q1q2 + q0q3) + 2*my*(0.5 - q1q1 - q3q3) + 2*mz*(q2q3 - q0q1);
	hz = 2*mx*(q1q3 - q0q2) + 2*my*(q2q3 + q0q1) + 2*mz*(0.5 - q1q1 - q2q2);         
	bx = sqrt((hx*hx) + (hy*hy));
	bz = hz;        
	
	// estimated direction of gravity and flux (v and w)
	vx = 2*(q1q3 - q0q2);
	vy = 2*(q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3;
	wx = 2*bx*(0.5 - q2q2 - q3q3) + 2*bz*(q1q3 - q0q2);
	wy = 2*bx*(q1q2 - q0q3) + 2*bz*(q0q1 + q2q3);
	wz = 2*bx*(q0q2 + q1q3) + 2*bz*(0.5 - q1q1 - q2q2);  
	
	// error is sum of cross product between reference direction of fields and direction measured by sensors
	ex = (ay*vz - az*vy) + (my*wz - mz*wy);
	ey = (az*vx - ax*vz) + (mz*wx - mx*wz);
	ez = (ax*vy - ay*vx) + (mx*wy - my*wx);
	
	// integral error scaled integral gain
	exInt = exInt + ex*Ki;
	eyInt = eyInt + ey*Ki;
	ezInt = ezInt + ez*Ki;
	
	// adjusted gyroscope measurements
	gx = sd->gx + Kp*ex + exInt;
	gy = sd->gy + Kp*ey + eyInt;
	gz = sd->gz + Kp*ez + ezInt;
	
	// integrate quaternion rate and normalise
	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
	q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
	q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
	q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  
	
	// normalise quaternion
	//inorm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	inorm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	if(inorm){
		q0 = q0 / inorm;
		q1 = q1 / inorm;
		q2 = q2 / inorm;
		q3 = q3 / inorm;
	}
	else{
		q0 = 0;
		q1 = 0;
		q2 = 0;
		q3 = 0;
	}
}
