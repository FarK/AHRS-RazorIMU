#include "quaternion.h"
#include "vector.h"
#include "mathUtils.h"

Quaternion::Quaternion(){
	q0 = 1;
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

void Quaternion::normalize(){
	float inorm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);

	q0 *= inorm;
	q1 *= inorm;
	q2 *= inorm;
	q3 *= inorm;
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

template <typename T>
Quaternion Quaternion::operator*(const Vector<T> &v){
	return Quaternion(
			       - q1*v.x - q2*v.y - q3*v.z,
			q0*v.x          + q2*v.z - q3*v.y,
			q0*v.y - q1*v.z          + q3*v.x,
			q0*v.z + q1*v.y - q2*v.x
			);
}

template <typename T>
Vector<float> Quaternion::rotateVector(const Vector<T> &v){
	//Auxiliary variables to reduce number of repeated operations
	//float q0q0 = q0*q0;
	float q0q1 = q0*q1;
	float q0q2 = q0*q2;
	float q0q3 = q0*q3;
	float q1q1 = q1*q1;
	float q1q2 = q1*q2;
	float q1q3 = q1*q3;
	float q2q2 = q2*q2;   
	float q2q3 = q2*q3;
	float q3q3 = q3*q3;          

	return Vector<float>(
			2*v.x*(0.5f - q2q2 - q3q3) + 2*v.y*(q0q3 + q1q2) + 2*v.z*(q1q3 - q0q2),
			2*v.x*(q1q2 - q0q3) + 2*v.y*(0.5f - q1q1 - q3q3) + 2*v.z*(q0q1 + q2q3),
			2*v.x*(q0q2 + q1q3) + 2*v.y*(q2q3 - q0q1) + 2*v.z*(0.5f - q1q1 + q2q2)
			);
} 

//Instanciación explicita
template Vector<float> Quaternion::rotateVector(const Vector<float> &v);
template Quaternion Quaternion::operator*(const Vector<float> &v);
