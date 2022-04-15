#include "pch.h"
#include "thread_matrix.h"

using namespace MathLib;

// This function is taken from the Chapter 2.3 LU Decomposition in the book
// Numerical recipes in C (1988).  Using Crout's method, a LU decomposed matrix
// is created.  Unlike the typical decomposition algorithm it stores both lower
// and upper in a single matrix, not storing the identity diagonal.  
// 
// Example:
//    Lower               Upper	    		DCMP
//     1  0  0  0     2  1  3  7       2  1  3  7
//    -2  1  0  0     0  1  2  1      -2  1  2  1
//     7 -3  1  0     0  0  7  3       7 -3  7  3
//    -1  3 .5  1     0  0  0 -1      -1  3 .5 -1
//
// The differences from the original version defined as ldcmp(a, n, indx, d) are:
// A	the array, is a private member variable arr
// N	is a private member variable row 
// D	in the original is a +1/-1.  In this version it is the value of the determinate
// INDX	same value
// 
// Note: A later update to the code may re-introduce the D parameter of +/- 1

template <typename T> T getRowScaling(T** arr, int row, size_t rows) {
	T scale = 0.0;
	for (int j = 0; j < rows; ++j) {
		T temp;
		if ((temp = abs(arr[row][j])) > scale) { scale = temp; }
	}
	return scale;
}

template <typename T> T MatrixBase<T>::ludcmp(T* indx) {
	using namespace std;

	if (!isSquare()) {
		throw MathException(MUST_BE_A_SQUARE);
	}

	unique_ptr<T[]> scaling(new T[rows]);		// Create scaling array
	size_t n = rows;
	T det = 1.0;

	// loop over rows to get the implicit scaling information
	for (int i = 0; i < n; ++i) {
		T scale = getRowScaling(arr, i, rows);
		if (scale == 0.0) { throw MathException(SINGULAR_MATRIX); }

		// No nonzero largest element
		scaling[i] = 1.0 / scale; // save the scaling
	}

	// Loop over columns using Crout's method
	for (int j = 0; j < n; ++j) {
		for (int i = 0; i < j; ++i) {
			T sum = arr[i][j];
			for (int k = 0; k < i; ++k) {
				sum -= arr[i][k] * arr[k][j];
			}
			arr[i][j] = sum;
		}

		// Search for the largest pivot element
		T big = 0.0;
		int imax;

		for (int i = j; i < n; ++i) {
			T sum = arr[i][j];
			for (int k = 0; k < j; k++) {
				sum -= arr[i][k] * arr[k][j];
			}
			arr[i][j] = sum;

			// is the figure of merit for the pivot better than the best so far
			T temp;
			if ((temp = scaling[i] * abs(sum)) > big) {
				big = temp;
				imax = i;
			}
		}

		// Interchange rows if needed
		if (j != imax) {
			for (int k = 0; k < n; ++k) {
				T dum = arr[imax][k];
				arr[imax][k] = arr[j][k];
				arr[j][k] = dum;
			}
			det = -det;
			scaling[imax] = scaling[j];	// also interchange the scale factor
		}

		indx[j] = imax;
		if (arr[j][j] == 0.0) { arr[j][j] = error_factor; }	// if pivot elem 0 substitute error_factor for 0
		if (j != n - 1) {
			T divider = arr[j][j];
			for (int i = j + 1; i < n; ++i) {
				arr[i][j] /= divider;
			}
		}
		det *= arr[j][j];
	}

	return det;
}
