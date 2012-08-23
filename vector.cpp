#include <stdint.h>
#include "vector.h"

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

//template <typename T>
//void Vector<T>::normalize(){
//	float inorm = invSqrt(x*x + y*y + z*z);
//	x *= inorm;
//	y *= inorm;
//	z *= inorm;
//}

//Instanciación explicita
template class Vector<int>;
template class Vector<float>;
