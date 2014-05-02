#pragma once

#include "vector.h"
#include <cassert>

/*!
This is a standard mathematical matrix class. Rows and cols are the dimensions of the matrix and T is the type of its elements.
The matrix is in column-major order. The rationale for this is that traditional mathematics uses m * v operator order, which means that,
for a translation matrix, the elements m(0, 3), m(1, 3), and m(2, 3) should contain the translation components. In addition, video cards
expect the translation components to be in elements 12, 13, and 14, which would then indicate a column-major order.
*/
template <int rows, int cols, typename T>
class Matrix
{
public:
	//! Default constructor.
	Matrix();

	//! Copy constructor.
	Matrix(Matrix<rows, cols, T> const & m);

	//! Returns a matrix where each element is zero.
	static Matrix<rows, cols, T> zero();

	//! Returns the identity matrix. Rows must equal cols.
	static Matrix<rows, cols, T> identity();

	//! Returns a matrix equivalent to the cross product with the first operand as v.
	static Matrix<rows, cols, T> crossProduct(Vector<rows, T> v);

	//! Access the element at row row and column col.
	T & operator()(int row, int col);

	//! Access the element at row row and column col.
	T const & operator()(int row, int col) const;

	//! Access the element at index i. Remember the matrix is column-major.
	T & operator [](int i);

	//! Access the element at index i. Remember the matrix is column-major.
	T const & operator [](int i) const;

	//! Assignment operator.
	Matrix<rows, cols, T> & operator =(Matrix<rows, cols, T> const & m);

	//! Get a pointer to the elements.
	T * ptr();
	
	//! Get a pointer to the elements.
	T const * ptr() const;

	//! Returns the transpose.
	Matrix<cols, rows, T> transpose() const;

	//! Returns this v, extending v either as a point(v3 = 1) or direction(v3 = 0). Rows must equal cols.
	Vector<rows - 1, T> transform(Vector<cols - 1, T> v, T v3) const;

	//! Returns v this. Used for dealing with row-major systems.
	Vector<cols, T> preMultiply(Vector<rows, T> v) const;

private:
	T c[rows * cols];
};

typedef Matrix<3, 3, float> Matrix33f;
typedef Matrix<3, 3, double> Matrix33d;
typedef Matrix<4, 4, float> Matrix44f;
typedef Matrix<4, 4, double> Matrix44d;

//! Returns true if each element in m0 is equal to its corresponding element in m1.
template <int rows, int cols, typename T> bool operator ==(Matrix<rows, cols, T> const & m0, Matrix<rows, cols, T> const & m1);

//! Returns true if any element in m0 is not equal to its corresponding element in m1.
template <int rows, int cols, typename T> bool operator !=(Matrix<rows, cols, T> const & m0, Matrix<rows, cols, T> const & m1);

//! Returns m0 + m1.
template <int rows, int cols, typename T> Matrix<rows, cols, T> operator +(Matrix<rows, cols, T> const & m0, Matrix<rows, cols, T> const & m1);

//! Returns m0 - m1.
template <int rows, int cols, typename T> Matrix<rows, cols, T> operator -(Matrix<rows, cols, T> const & m0, Matrix<rows, cols, T> const & m1);

//! Returns m0 m1.
template <int rows, int mid, int cols, typename T> Matrix<rows, cols, T> operator *(Matrix<rows, mid, T> const & m0, Matrix<mid, cols, T> const & m1);

//! Returns a m. There is no m a syntax to be consistent with standard mathematical formula writing order.
template <int rows, int cols, typename T> Matrix<rows, cols, T> operator *(T a, Matrix<rows, cols, T> const & m);

//! Returns m v.
template <int rows, int cols, typename T> Vector<rows, T> operator *(Matrix<rows, cols, T> const & m, Vector<cols, T> v);

// Template Implementations

template <int rows, int cols, typename T>
Matrix<rows, cols, T>::Matrix()
{
	assert(rows > 0 && cols > 0);
}

template <int rows, int cols, typename T>
Matrix<rows, cols, T>::Matrix(Matrix<rows, cols, T> const & m)
{
	assert(rows > 0 && cols > 0);
	*this = m;
}

template <int rows, int cols, typename T>
Matrix<rows, cols, T> Matrix<rows, cols, T>::zero()
{
	Matrix<rows, cols, T> r;
	int size = rows * cols;
	for(int i = 0; i < size; ++i)
	{
		r.c[i] =(T)0;
	}
	return r;
}

template <int rows, int cols, typename T>
Matrix<rows, cols, T> Matrix<rows, cols, T>::identity()
{
	assert(rows == cols);
	Matrix<rows, cols, T> r = zero();
	int size = rows * cols;
	for(int i = 0; i < size; i += rows + 1)
	{
		r.c[i] =(T)1;
	}
	return r;
}

template <int rows, int cols, typename T>
Matrix<rows, cols, T> Matrix<rows, cols, T>::crossProduct(Vector<rows, T> v)
{
	assert(rows == 3 && cols == 3);
	Matrix<rows, cols, T> r;
	r.c[0 * rows + 0] = 0;		 r.c[1 * rows + 0] = -v[2]; r.c[2 * rows + 0] = +v[1];
	r.c[0 * rows + 1] = +v[2]; r.c[1 * rows + 1] = 0;		 r.c[2 * rows + 1] = -v[0];
	r.c[0 * rows + 2] = -v[1]; r.c[1 * rows + 2] = +v[0]; r.c[2 * rows + 2] = 0;
	return r;
}

template <int rows, int cols, typename T>
T & Matrix<rows, cols, T>::operator()(int row, int col)
{
	assert(0 <= row && row < rows && 0 <= col && col < cols);
	return c[col * rows + row];
}

template <int rows, int cols, typename T>
T const & Matrix<rows, cols, T>::operator()(int row, int col) const
{
	assert(0 <= row && row < rows && 0 <= col && col < cols);
	return c[col * rows + row];
}

template <int rows, int cols, typename T>
T & Matrix<rows, cols, T>::operator [](int i)
{
	assert(0 <= i && i < rows * cols);
	return c[i];
}

template <int rows, int cols, typename T>
T const & Matrix<rows, cols, T>::operator [](int i) const
{
	assert(0 <= i && i < rows * cols);
	return c[i];
}

template <int rows, int cols, typename T>
Matrix<rows, cols, T> & Matrix<rows, cols, T>::operator =(Matrix<rows, cols, T> const & m)
{
	int size = rows * cols;
	for(int i = 0; i < size; ++i)
	{
		c[i] = m.c[i];
	}
	return *this;
}

template <int rows, int cols, typename T>
T * Matrix<rows, cols, T>::ptr()
{
	return c;
}

template <int rows, int cols, typename T>
T const * Matrix<rows, cols, T>::ptr() const
{
	return c;
}

template <int rows, int cols, typename T>
Matrix<cols, rows, T> Matrix<rows, cols, T>::transpose() const
{
	Matrix<cols, rows, T> r;
	for(int i = 0; i < rows; ++i)
	{
		int icols = i * cols;
		for(int j = 0; j < cols; ++j)
		{
			r(icols + j) = c[j * rows + i];
		}
	}
	return r;
}

template <int rows, int cols, typename T>
Vector<rows - 1, T> Matrix<rows, cols, T>::transform(Vector<cols - 1, T> v, T v3) const
{
	assert(rows == cols &&(v3 == 0 || v3 == 1));
	Vector<rows - 1, T> r;
	for(int i = 0; i < rows - 1; ++i)
	{
		r[i] =(T)0;
	}
	for(int k = 0; k < cols - 1; ++k)
	{
		int krows = k * rows;
		T v_k = v[k];
		for(int i = 0; i < rows - 1; ++i)
		{
			r[i] += c[krows + i] * v_k;
		}
	}
	int krows =(cols - 1) * rows;
	for(int i = 0; i < rows - 1; i++)
	{
		r[i] += c[krows + i] * v3;
	}
	return r;
}

template <int rows, int cols, typename T>
Vector<cols, T> Matrix<rows, cols, T>::preMultiply(Vector<rows, T> v) const
{
	Vector<cols, T> r;
	for(int i = 0; i < cols; ++i)
	{
		r[i] =(T)0;
	}
	for(int i = 0; i < cols; ++i)
	{
		int irows = i * rows;
		for(int k = 0; k < rows; ++k)
		{
			r[i] += v[k] * c[irows + k];
		}
	}
	return r;
}

template <int rows, int cols, typename T>
bool operator ==(Matrix<rows, cols, T> const & m0, Matrix<rows, cols, T> const & m1)
{
	int size = rows * cols;
	for(int i = 0; i < size; ++i)
	{
		if(m0[i] != m1[i])
		{
			return false;
		}
	}
	return true;
}

template <int rows, int cols, typename T>
bool operator !=(Matrix<rows, cols, T> const & m0, Matrix<rows, cols, T> const & m1)
{
	return !(m0 == m1);
}

template <int rows, int cols, typename T>
Matrix<rows, cols, T> operator +(Matrix<rows, cols, T> const & m0, Matrix<rows, cols, T> const & m1)
{
	Matrix<rows, cols, T> r;
	int size = rows * cols;
	for(int i = 0; i < size; ++i)
	{
		r[i] = m0[i] + m1[i];
	}
	return r;
}

template <int rows, int cols, typename T>
Matrix<rows, cols, T> operator -(Matrix<rows, cols, T> const & m0, Matrix<rows, cols, T> const & m1)
{
	Matrix<rows, cols, T> r;
	int size = rows * cols;
	for(int i = 0; i < size; ++i)
	{
		r[i] = m0[i] - m1[i];
	}
	return r;
}

template <int rows, int mid, int cols, typename T>
Matrix<rows, cols, T> operator *(Matrix<rows, mid, T> const & m0, Matrix<mid, cols, T> const & m1)
{
	Matrix<rows, cols, T> r;
	for(int j = 0; j < cols; ++j)
	{
		int jrows = j * rows;
		int jmid = j * mid;
		for(int i = 0; i < rows; ++i)
		{
			r[jrows + i] =(T)0;
		}
		for(int k = 0; k < mid; ++k)
		{
			int krows = k * rows;
			T m1_jmidk = m1[jmid + k];
			for(int i = 0; i < rows; ++i)
			{
				r[jrows + i] += m0[krows + i] * m1_jmidk;
			}
		}
	}
	return r;
}

template <int rows, int cols, typename T>
Matrix<rows, cols, T> operator *(T a, Matrix<rows, cols, T> const & m)
{
	Matrix<rows, cols, T> r;
	int size = rows * cols;
	for(int i = 0; i < size; ++i)
	{
		r[i] = a * m[i];
	}
	return r;
}

template <int rows, int cols, typename T>
Vector<rows, T> operator *(Matrix<rows, cols, T> const & m, Vector<cols, T> v)
{
	Vector<rows, T> r;
	for(int i = 0; i < rows; ++i)
	{
		r[i] =(T)0;
	}
	for(int k = 0; k < cols; ++k)
	{
		int krows = k * rows;
		T v_k = v[k];
		for(int i = 0; i < rows; ++i)
		{
			r[i] += m[krows + i] * v_k;
		}
	}
	return r;
}

