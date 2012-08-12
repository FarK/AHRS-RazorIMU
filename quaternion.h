#ifndef QUATERNION_H
#define QUATERNION_H

class Quaternion{
	public:
		//CONSTRUCTORS
		Quaternion();
		Quaternion(float q0, float q1, float q2, float q3);

		//Quaternion conj();
	
		//Sum term by term
		Quaternion operator+(const Quaternion &q);
		//Hamilton rule product
		Quaternion operator*(const float f);
		//Product with a scalar
		Quaternion operator*(const Quaternion &q);

		//Rotate vector according to quaternion
		Quaternion rotateVector(float dx, float dy, float dz);

		//ATRIBUTES
		float q0,q1,q2,q3;
};

#endif
