#ifndef VECTOR_H
#define VECTOR_H

template <typename T>
class Vector{
	public:
		//CONSTRUCTORS
		Vector();
		Vector(T x, T y, T z);
		
		//ATRIBUTES
		T x,y,z;

		//Norm
		void normalize();
};

#endif
