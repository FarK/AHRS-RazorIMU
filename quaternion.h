#ifndef QUATERNION_H
#define QUATERNION_H

#include "vector.h"

class Quaternion{
	public:
		//CONSTRUCTORS
		Quaternion();
		Quaternion(float q0, float q1, float q2, float q3);

		//Conjuge this quaternion
		void conjugate();
		//Normalize this cuaternion
		void normalize();
	
		//Sum term by term
		Quaternion operator+(const Quaternion &q);

		//Product with a scalar
		Quaternion operator*(const float f);

		//Hamilton rule product
		Quaternion operator*(const Quaternion &q);

		//Hamilton rule product with vector
		template <typename T>
		Quaternion operator*(const Vector<T> &q);

		//Rotate vector according to quaternion
		template <typename T>
		Vector<float> rotateVector(const Vector<T> &vector);

		//ATRIBUTES
		float q0,q1,q2,q3;
};

#endif
