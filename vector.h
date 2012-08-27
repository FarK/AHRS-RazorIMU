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

		//Sum term by term
		template <typename T2>
		Vector<T> operator+(const Vector<T2> &v) const;

		//Division term by term
		Vector<T> operator/(const float &scalar) const;

		//Dot product
		template <typename T2>
		T operator*(const Vector<T2> &v) const;

		//Product whit scalar
		template <typename T2>
		Vector<T> operator*(T2 scalar) const;
};

#endif
