#include "pch.h"
#include <array>
#include <algorithm>
#include "matrixbase.h"
#include "mathexception.h"

using namespace MathLib;

template <typename T> void MatrixBase<T>::init() {
	if (columns == 0) {
		columns = rows;
	}
	size_t num_cells = rows * columns;  // Calculate the number of cells
	buffer = new T[num_cells];          // Allocate a single contiguous array for each cell
	row = new T * [num_cells];           // Allocate an array of pointers to the first element of each row

	T* curr = buffer;
	for (size_t i = 0; i < rows; i++) {
		row[i] = curr;      // store the address of row[i][0]
		curr += columns;    // advance to the next column
	}
}

template MatrixBase<double>::MatrixBase(size_t rows, size_t columns);
template MatrixBase<long double>::MatrixBase(size_t rows, size_t columns);

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
				sum += row[i][k] * m2.row[k][j];
			}
			result.row[i][j] = sum;
		}
	}

	return result;
}

template MatrixBase<double> &MatrixBase<double>::product(MatrixBase<double>& m2, MatrixBase<double>& result);
template MatrixBase<long double>& MatrixBase<long double>::product(MatrixBase<long double>& m2, MatrixBase<long double>& result);

template <typename T> void MatrixBase<T>::LU(MatrixBase<T>& dcmp, T *indx) {
	if (dcmp.rows != rows || dcmp.columns != columns) {
		throw MathException("Matrixe sizes don't match");
	}

	if (rows != columns) {
		throw MathException("Matrixes not square");
	}

	dcmp = *this;

	//T indx[rows];
	dcmp = *this;

	dcmp.ludcmp(indx);
}

template void MatrixBase<double>::LU(MatrixBase<double>& dcmp, double* indx);
template void MatrixBase<long double>::LU(MatrixBase<long double>& dcmp, long double* indx);

// This function is taken from the Chapter 2.3 LU Decomposition in the book
// Numerical recipes in C It creates a LU decomped matrix inside a single LU matrix
template <typename T> T MatrixBase<T>::ludcmp(T *indx) {
	int i, imax, j, k;
	T dum, temp;

	using namespace std;

	cout << "Making vv array of " << rows << " rows\n";
	unique_ptr<T[]> vv(new T[rows]);
	size_t n = rows;
	T det = 1.0;

	cout << "ludcmp:\n" << *this << std::endl;
	// loop over rows to get the implicit scaling information
	for (i = 0; i < n; ++i) {
		T big = 0.0;
		for (j = 0; j < n; ++j) {
			if ((temp = abs(row[i][j])) > big) { big = temp; }
		}
		if (big == 0.0) { throw MathException(SINGULAR_MATRIX);}
		// No nonzero largest element
		vv[i] = 1.0 / big; // save the scaling
	}

	cout << "Doung Crout's method." << endl;
	// Loop over columns of Crout's method
	for (j = 0; j < n; ++j) {
		for (i = 0; i < j; ++i) {
			T sum = row[i][j];
			for (k = 0; k < i; ++k) {
				sum -= row[i][k] * row[k][j];
			}
			row[i][j] = sum;
		}
		T big = 0.0;	// initialize for the search for largest pivot element
		for (i = j; i < n; ++i) {
			T sum = row[i][j];
			for (k = 0; k < j; k++) {
				sum -= row[i][k] * row[k][j];
			}
			row[i][j] = sum;

			// is the figure of merit for the pivot better than the best so far
			if ((dum = vv[i] * abs(sum)) > big) { 
				big = dum;
				imax = i;
			}
		}
		
		// Interchange rows if needed
		if (j != imax) {
			for (k = 0; k < n; ++k) {
				dum = row[imax][k];
				row[imax][k] = row[j][k];
				row[j][k] = dum;
			}
			det = -det;
			vv[imax] = vv[j];	// also interchange the scale factor
		}
		indx[j] = imax;
		if (row[j][j] == 0.0) { row[j][j] = error_factor; }	// if pivot elem 0 substitute error_factor for 0
		if (j != n-1) {
			dum = 1.0 / row[j][j];
			for (i = j + 1; i < n; ++i) {
				row[i][j] *= dum;
			}
		}
		det *= row[j][j];
	}

	cout << "vv: {";
	for (i = 0; i < n; ++i) {
		if (i) { cout << ", "; }
		cout << vv[i];
	}
	cout << "}\n";

	cout << "after:\n" << *this << endl;
	return det;
}

template double MatrixBase<double>::ludcmp(double* indx);

template <typename T> T MatrixBase<T>::determinate() {
	if (columns != rows) {
		throw MathException(MUST_BE_A_SQUARE);
	}

	T det = 1.0;

	for (size_t i = 0; i < columns - 1; ++i) {
		// Partial pivot: find row r with the largest element in column i
		size_t r = i;
		T maxR = abs(row[i][i]);
		for (size_t k = i + 1; k < columns; ++k) {
			T val = abs(row[k][i]);
			if (val > maxR) {
				r = k;
				maxR = val;
			}
		}

		// swap row if R != i and set det to -det
		if (r != i) {
			for (size_t j = i; j < columns; ++j) {
				std::swap(row[i][j], row[r][j]);
				det = -det;
			}
		}

		// Make upper triangle
		T curr_diag = row[i][i];
		if (abs(curr_diag) < error_factor) return 0.0;

		for (size_t r = i + 1; r < columns; ++r) {
			T multiple = row[r][i] / curr_diag;	// multiple of row i to clear element in ith column
			for (size_t j = i; j < columns; ++j) {
				row[r][j] -= multiple * row[i][j];
			}
		}
		det *= curr_diag; // update determinate by current diagonal
	}

	det *= row[rows - 1][columns - 1];

	return det;
}


template double MatrixBase<double>::determinate();
template long double MatrixBase<long double>::determinate();