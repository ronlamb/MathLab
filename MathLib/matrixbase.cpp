#include "pch.h"
#include <array>
#include <algorithm>
#include "matrixbase.h"
#include "mathexception.h"

using namespace MathLib;

/*
 * Initialize the matrix.
 */
template <typename T> void MatrixBase<T>::init() {
	if (columns == 0) {
		columns = rows;
	}
	size_t num_cells = rows * columns;  // Calculate the number of cells
	buffer = new T[num_cells];          // Allocate a single contiguous array for each cell
	arr = new T * [rows];				// Allocate an array of pointers to the first element of each row

	T* curr = buffer;
	for (size_t i = 0; i < rows; i++) {
		arr[i] = curr;      // store the address of arr[i][0]
		curr += columns;    // advance to the next column
	}
}

// static const T error_factor = std::numeric_limits<T>::min() * 100;
//double X = std::numeric_limits<double>::min() * 100;
//template const double MatrixBase<double>::error_factor = 0.0;
// template long double MatrixBase<long double>::error_factor = std::numeric_limits<long double>::min() * 100;
template MatrixBase<double>::MatrixBase(size_t rows, size_t columns);
template MatrixBase<long double>::MatrixBase(size_t rows, size_t columns);

template void MatrixBase<double>::init();
template  void MatrixBase<long double>::init();

template <typename T> MatrixBase<T>& MatrixBase<T>::product(MatrixBase<T>& m2, MatrixBase<T>& result) {
	using namespace std;
	if (columns != m2.rows) {
		throw MathException(COLUMNS_MUST_MATCH_ROWS);
	}

	cout << "result size = " << result.rows << "x" << result.columns << endl;
	if (rows != result.rows || m2.columns != result.columns) {
		throw MathException("Result must be an " + to_string(rows) + " by" + to_string(m2.columns) + " matrix.");
	}

	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < m2.columns; j++) {
			T sum = 0.0;
			for (size_t k = 0; k < columns; k++) {
				sum += arr[i][k] * m2.arr[k][j];
			}
			result.arr[i][j] = sum;
		}
	}

	return result;
}

template MatrixBase<double> &MatrixBase<double>::product(MatrixBase<double>& m2, MatrixBase<double>& result);
template MatrixBase<long double>& MatrixBase<long double>::product(MatrixBase<long double>& m2, MatrixBase<long double>& result);

template <typename T> T MatrixBase<T>::LU(MatrixBase<T>& dcmp, T *indx) {
	if (dcmp.rows != rows || dcmp.columns != columns) {
		throw MathException("Matrix sizes don't match");
	}

	if (rows != columns) {
		throw MathException("Matrixes not square");
	}

	dcmp = *this;

	//T indx[rows];
	dcmp = *this;

	return dcmp.ludcmp(indx);
}

template double MatrixBase<double>::LU(MatrixBase<double>& dcmp, double* indx);
template long double MatrixBase<long double>::LU(MatrixBase<long double>& dcmp, long double* indx);

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
template <typename T> T MatrixBase<T>::ludcmp(T *indx) {
	using namespace std;

	if (!isSquare()) {
		throw MathException(MUST_BE_A_SQUARE);
	}

	unique_ptr<T[]> scaling(new T[rows]);		// Create scaling array
	size_t n = rows;
	T det = 1.0;

	// loop over rows to get the implicit scaling information
	for (int i = 0; i < n; ++i) {
		T big = 0.0;
		for (int j = 0; j < n; ++j) {
			T temp;
			if ((temp = abs(arr[i][j])) > big) { big = temp; }
		}
		if (big == 0.0) { throw MathException(SINGULAR_MATRIX);}

		// No nonzero largest element
		scaling[i] = 1.0 / big; // save the scaling
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
		if (j != n-1) {
			T divider = arr[j][j];
			for (int i = j + 1; i < n; ++i) {
				arr[i][j] /= divider;
			}
		}
		det *= arr[j][j];
	}

	return det;
}

template double MatrixBase<double>::ludcmp(double* indx);
template long double MatrixBase<long double>::ludcmp(long double* indx);

// Determinate function
// Simplified version of the determinate function.  Crout's version ludcmp also
// Returns the determinate, for when both LU and determinate is needed.
template <typename T> T MatrixBase<T>::determinate() {
	if (!isSquare()) {
		throw MathException(MUST_BE_A_SQUARE);
	}

	T det = 1.0;

	for (size_t i = 0; i < columns - 1; ++i) {
		// Partial pivot: find row r with the largest element in column i
		size_t r = i;
		T maxR = abs(arr[i][i]);
		for (size_t k = i + 1; k < columns; ++k) {
			T val = abs(arr[k][i]);
			if (val > maxR) {
				r = k;
				maxR = val;
			}
		}

		// swap row if R != i and set det to -det
		if (r != i) {
			for (size_t j = i; j < columns; ++j) {
				std::swap(arr[i][j], arr[r][j]);
				det = -det;
			}
		}

		// Make upper triangle
		T curr_diag = arr[i][i];
		if (abs(curr_diag) < error_factor) return 0.0;

		for (size_t r = i + 1; r < columns; ++r) {
			T multiple = arr[r][i] / curr_diag;	// multiple of row i to clear element in ith column
			for (size_t j = i; j < columns; ++j) {
				arr[r][j] -= multiple * arr[i][j];
			}
		}
		det *= curr_diag; // update determinate by current diagonal
	}

	det *= arr[rows - 1][columns - 1];

	return det;
}

template double MatrixBase<double>::determinate();
template long double MatrixBase<long double>::determinate();