#include <stdint.h>
#include "vector.h"
#include "mathUtils.h"

template <typename T>
Vector<T>::Vector(){
	x = 0;
	y = 0;
	z = 0;
}

template <typename T>
Vector<T>::Vector(T x, T y, T z){
	this->x = x;
	this->y = y;
	this->z = z;
}

template <typename T>
void Vector<T>::normalize(){
	float inorm = invSqrt(x*x + y*y + z*z);
	x *= inorm;
	y *= inorm;
	z *= inorm;
}

template <typename T>
float Vector<T>::iNorm() const{
	return invSqrt(x*x + y*y + z*z);
}

template<typename T> template<typename T2>
T Vector<T>::operator*(const Vector<T2> &v) const{
	return x*v.x + y*v.y + z*v.z;
}

template<typename T>
Vector<T> Vector<T>::operator*(const T scalar) const{
	return Vector(x*scalar, y*scalar, z*scalar);
}

//Instanciación explicita
template class Vector<int>;
template class Vector<float>;
template float Vector<float>::operator*(const Vector<float> &v) const;
