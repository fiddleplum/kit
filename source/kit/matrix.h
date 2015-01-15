#pragma once

#include "coord.h"

/*
	This is a standard mathematical matrix class. Rows and cols are the dimensions of the matrix and T is the type of its elements.
	The matrix is in column-major order. The rationale for this is that traditional mathematics uses m * v operator order, which means that,
	for a translation matrix, the elements m(0, 3), m(1, 3), and m(2, 3) should contain the translation components. In addition, video cards
	expect the translation components to be in elements 12, 13, and 14, which would then indicate a column-major order.
*/
template <unsigned int rows, unsigned int cols, typename T>
class Matrix
{
public:
	// Default constructor.
	Matrix();

	// Copy constructor.
	Matrix(Matrix<rows, cols, T> const & m);

	// Returns a matrix where each element is zero.
	static Matrix<rows, cols, T> zero();

	// Returns the identity matrix. Rows must equal cols.
	static Matrix<rows, cols, T> identity();

	// Returns a matrix equivalent to the cross product with the first operand as v.
	static Matrix<rows, cols, T> crossProduct(Coord<rows, T> v);

	// Access the element at row row and column col.
	T & operator()(unsigned int row, unsigned int col);

	// Access the element at row row and column col.
	T const & operator()(unsigned int row, unsigned int col) const;

	// Access the element at index i. Remember the matrix is column-major.
	T & operator [](unsigned int i);

	// Access the element at index i. Remember the matrix is column-major.
	T const & operator [](unsigned int i) const;

	// Assignment operator.
	Matrix<rows, cols, T> & operator = (Matrix<rows, cols, T> const & m);

	// Get a pointer to the elements.
	T * ptr();

	// Get a pointer to the elements.
	T const * ptr() const;

	// Returns the transpose.
	Matrix<cols, rows, T> transpose() const;

	// Returns this v, extending v either as a point(v3 = 1) or direction(v3 = 0). Rows must equal cols.
	Coord < rows - 1, T > transform(Coord < cols - 1, T > v, T v3) const;

	// Returns v this. Used for dealing with row-major systems.
	Coord<cols, T> preMultiply(Coord<rows, T> v) const;

private:
	T c[rows * cols];
};

typedef Matrix<3, 3, float> Matrix33f;
typedef Matrix<3, 3, double> Matrix33d;
typedef Matrix<4, 4, float> Matrix44f;
typedef Matrix<4, 4, double> Matrix44d;

// Returns true if each element in m0 is equal to its corresponding element in m1.
template <unsigned int rows, unsigned int cols, typename T> bool operator == (Matrix<rows, cols, T> const & m0, Matrix<rows, cols, T> const & m1);

// Returns true if any element in m0 is not equal to its corresponding element in m1.
template <unsigned int rows, unsigned int cols, typename T> bool operator != (Matrix<rows, cols, T> const & m0, Matrix<rows, cols, T> const & m1);

// Returns m0 + m1.
template <unsigned int rows, unsigned int cols, typename T> Matrix<rows, cols, T> operator + (Matrix<rows, cols, T> const & m0, Matrix<rows, cols, T> const & m1);

// Returns m0 - m1.
template <unsigned int rows, unsigned int cols, typename T> Matrix<rows, cols, T> operator - (Matrix<rows, cols, T> const & m0, Matrix<rows, cols, T> const & m1);

// Returns m0 m1.
template <unsigned int rows, unsigned int mid, unsigned int cols, typename T> Matrix<rows, cols, T> operator * (Matrix<rows, mid, T> const & m0, Matrix<mid, cols, T> const & m1);

// Returns a m. There is no m a syntax to be consistent with standard mathematical formula writing order.
template <unsigned int rows, unsigned int cols, typename T> Matrix<rows, cols, T> operator * (T a, Matrix<rows, cols, T> const & m);

// Returns m v.
template <unsigned int rows, unsigned int cols, typename T> Coord<rows, T> operator * (Matrix<rows, cols, T> const & m, Coord<cols, T> v);

// Template Implementations

template <unsigned int rows, unsigned int cols, typename T>
Matrix<rows, cols, T>::Matrix()
{
	assert(rows > 0 && cols > 0);
}

template <unsigned int rows, unsigned int cols, typename T>
Matrix<rows, cols, T>::Matrix(Matrix<rows, cols, T> const & m)
{
	assert(rows > 0 && cols > 0);
	*this = m;
}

template <unsigned int rows, unsigned int cols, typename T>
Matrix<rows, cols, T> Matrix<rows, cols, T>::zero()
{
	Matrix<rows, cols, T> r;
	unsigned int size = rows * cols;
	for(unsigned int i = 0; i < size; ++i)
	{
		r.c[i] = (T)0;
	}
	return r;
}

template <unsigned int rows, unsigned int cols, typename T>
Matrix<rows, cols, T> Matrix<rows, cols, T>::identity()
{
	assert(rows == cols);
	Matrix<rows, cols, T> r = zero();
	unsigned int size = rows * cols;
	for(unsigned int i = 0; i < size; i += rows + 1)
	{
		r.c[i] = (T)1;
	}
	return r;
}

template <unsigned int rows, unsigned int cols, typename T>
Matrix<rows, cols, T> Matrix<rows, cols, T>::crossProduct(Coord<rows, T> v)
{
	assert(rows == 3 && cols == 3);
	Matrix<rows, cols, T> r;
	r.c[0 * rows + 0] = 0;
	r.c[1 * rows + 0] = -v[2];
	r.c[2 * rows + 0] = +v[1];
	r.c[0 * rows + 1] = +v[2];
	r.c[1 * rows + 1] = 0;
	r.c[2 * rows + 1] = -v[0];
	r.c[0 * rows + 2] = -v[1];
	r.c[1 * rows + 2] = +v[0];
	r.c[2 * rows + 2] = 0;
	return r;
}

template <unsigned int rows, unsigned int cols, typename T>
T & Matrix<rows, cols, T>::operator()(unsigned int row, unsigned int col)
{
	if(row >= rows || col >= cols)
	{
		throw std::exception();
	}
	return c[col * rows + row];
}

template <unsigned int rows, unsigned int cols, typename T>
T const & Matrix<rows, cols, T>::operator()(unsigned int row, unsigned int col) const
{
	if(row >= rows || col >= cols)
	{
		throw std::exception();
	}
	return c[col * rows + row];
}

template <unsigned int rows, unsigned int cols, typename T>
T & Matrix<rows, cols, T>::operator [](unsigned int i)
{
	if(i >= rows * cols)
	{
		throw std::exception();
	}
	return c[i];
}

template <unsigned int rows, unsigned int cols, typename T>
T const & Matrix<rows, cols, T>::operator [](unsigned int i) const
{
	if(i >= rows * cols)
	{
		throw std::exception();
	}
	return c[i];
}

template <unsigned int rows, unsigned int cols, typename T>
Matrix<rows, cols, T> & Matrix<rows, cols, T>::operator = (Matrix<rows, cols, T> const & m)
{
	unsigned int size = rows * cols;
	for(unsigned int i = 0; i < size; ++i)
	{
		c[i] = m.c[i];
	}
	return *this;
}

template <unsigned int rows, unsigned int cols, typename T>
T * Matrix<rows, cols, T>::ptr()
{
	return c;
}

template <unsigned int rows, unsigned int cols, typename T>
T const * Matrix<rows, cols, T>::ptr() const
{
	return c;
}

template <unsigned int rows, unsigned int cols, typename T>
Matrix<cols, rows, T> Matrix<rows, cols, T>::transpose() const
{
	Matrix<cols, rows, T> r;
	for(unsigned int i = 0; i < rows; ++i)
	{
		unsigned int icols = i * cols;
		for(unsigned int j = 0; j < cols; ++j)
		{
			r(icols + j) = c[j * rows + i];
		}
	}
	return r;
}

template <unsigned int rows, unsigned int cols, typename T>
Coord < rows - 1, T > Matrix<rows, cols, T>::transform(Coord < cols - 1, T > v, T v3) const
{
	assert(rows == cols && rows > 1);
	Coord < rows - 1, T > r;
	for(unsigned int i = 0; i < rows - 1; ++i)
	{
		r[i] = (T)0;
	}
	for(unsigned int k = 0; k < cols - 1; ++k)
	{
		unsigned int krows = k * rows;
		T v_k = v[k];
		for(unsigned int i = 0; i < rows - 1; ++i)
		{
			r[i] += c[krows + i] * v_k;
		}
	}
	unsigned int krows = (cols - 1) * rows;
	for(unsigned int i = 0; i < rows - 1; i++)
	{
		r[i] += c[krows + i] * v3;
	}
	return r;
}

template <unsigned int rows, unsigned int cols, typename T>
Coord<cols, T> Matrix<rows, cols, T>::preMultiply(Coord<rows, T> v) const
{
	Coord<cols, T> r;
	for(int i = 0; i < cols; ++i)
	{
		r[i] = (T)0;
	}
	for(unsigned int i = 0; i < cols; ++i)
	{
		unsigned int irows = i * rows;
		for(unsigned int k = 0; k < rows; ++k)
		{
			r[i] += v[k] * c[irows + k];
		}
	}
	return r;
}

template <unsigned int rows, unsigned int cols, typename T>
bool operator == (Matrix<rows, cols, T> const & m0, Matrix<rows, cols, T> const & m1)
{
	unsigned int size = rows * cols;
	for(unsigned int i = 0; i < size; ++i)
	{
		if(m0[i] != m1[i])
		{
			return false;
		}
	}
	return true;
}

template <unsigned int rows, unsigned int cols, typename T>
bool operator != (Matrix<rows, cols, T> const & m0, Matrix<rows, cols, T> const & m1)
{
	return !(m0 == m1);
}

template <unsigned int rows, unsigned int cols, typename T>
Matrix<rows, cols, T> operator + (Matrix<rows, cols, T> const & m0, Matrix<rows, cols, T> const & m1)
{
	Matrix<rows, cols, T> r;
	unsigned int size = rows * cols;
	for(unsigned int i = 0; i < size; ++i)
	{
		r[i] = m0[i] + m1[i];
	}
	return r;
}

template <unsigned int rows, unsigned int cols, typename T>
Matrix<rows, cols, T> operator - (Matrix<rows, cols, T> const & m0, Matrix<rows, cols, T> const & m1)
{
	Matrix<rows, cols, T> r;
	unsigned int size = rows * cols;
	for(unsigned int i = 0; i < size; ++i)
	{
		r[i] = m0[i] - m1[i];
	}
	return r;
}

template <unsigned int rows, unsigned int mid, unsigned int cols, typename T>
Matrix<rows, cols, T> operator * (Matrix<rows, mid, T> const & m0, Matrix<mid, cols, T> const & m1)
{
	Matrix<rows, cols, T> r;
	for(unsigned int j = 0; j < cols; ++j)
	{
		unsigned int jrows = j * rows;
		unsigned int jmid = j * mid;
		for(unsigned int i = 0; i < rows; ++i)
		{
			r[jrows + i] = (T)0;
		}
		for(unsigned int k = 0; k < mid; ++k)
		{
			unsigned int krows = k * rows;
			T m1_jmidk = m1[jmid + k];
			for(unsigned int i = 0; i < rows; ++i)
			{
				r[jrows + i] += m0[krows + i] * m1_jmidk;
			}
		}
	}
	return r;
}

template <unsigned int rows, unsigned int cols, typename T>
Matrix<rows, cols, T> operator * (T a, Matrix<rows, cols, T> const & m)
{
	Matrix<rows, cols, T> r;
	unsigned int size = rows * cols;
	for(unsigned int i = 0; i < size; ++i)
	{
		r[i] = a * m[i];
	}
	return r;
}

template <unsigned int rows, unsigned int cols, typename T>
Coord<rows, T> operator * (Matrix<rows, cols, T> const & m, Coord<cols, T> v)
{
	Coord<rows, T> r;
	for(unsigned int i = 0; i < rows; ++i)
	{
		r[i] = (T)0;
	}
	for(unsigned int k = 0; k < cols; ++k)
	{
		unsigned int krows = k * rows;
		T v_k = v[k];
		for(unsigned int i = 0; i < rows; ++i)
		{
			r[i] += m[krows + i] * v_k;
		}
	}
	return r;
}

