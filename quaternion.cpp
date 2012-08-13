#include "quaternion.h"

Quaternion::Quaternion(){
	q0 = 0;
	q1 = 0;
	q2 = 0;
	q3 = 0;
}

Quaternion::Quaternion(float q0, float q1, float q2, float q3){
	this->q0=q0;
	this->q1=q1;
	this->q2=q2;
	this->q3=q3;
}

void Quaternion::conjugate(){
	q1 = -q1;
	q2 = -q2;
	q3 = -q3;
}

Quaternion Quaternion::operator+(const Quaternion &q){
	return Quaternion(q0+q.q0, q1+q.q1, q2+q.q2, q3+q.q3);
}

Quaternion Quaternion::operator*(const float f){
	return Quaternion(q0*f, q1*f, q2*f, q3*f);
}

Quaternion Quaternion::operator*(const Quaternion &q){
	return Quaternion(
			q0*q.q0 - q1*q.q1 - q2*q.q2 - q3*q.q3,
			q0*q.q1 + q1*q.q0 + q2*q.q3 - q3*q.q2,
			q0*q.q2 - q1*q.q3 + q2*q.q0 + q3*q.q1,
			q0*q.q3 + q1*q.q2 - q2*q.q1 + q3*q.q0
			);
}

Quaternion Quaternion::rotateVector(float dx, float dy, float dz){
	//Auxiliary variables to reduce number of repeated operations
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

	return Quaternion(
			0.0f,
			2*dx*(0.5f - q2q2 - q3q3) + 2*dy*(q0q3 + q1q2) + 2*dz*(q1q3 - q0q2),
			2*dx*(q1q2 - q0q3) + 2*dy*(0.5f - q1q1 - q3q3) + 2*dz*(q0q1 + q2q3),
			2*dx*(q0q2 + q1q3) + 2*dy*(q2q3 - q0q1) + 2*dz*(0.5f - q1q1 + q2q2)
			);
} 
