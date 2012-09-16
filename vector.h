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
		float iNorm() const;

		//Product with scalar
		Vector<T> operator*(const T scalar) const;

		//Dot product
		template <typename T2>
		T operator*(const Vector<T2> &v) const;
};

#endif
