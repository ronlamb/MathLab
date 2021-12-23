#include "pch.h"
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

//template <typename T> MatrixBase<T>::MatrixBase(size_t rows, size_t columns) {
//	
//}

template MatrixBase<double>::MatrixBase(size_t rows, size_t columns);
template MatrixBase<long double>::MatrixBase(size_t rows, size_t columns);

template <typename T> MatrixBase<T>& MatrixBase<T>::Product(MatrixBase<T>& m2, MatrixBase<T>& result) {
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

template MatrixBase<double> &MatrixBase<double>::Product(MatrixBase<double>& m2, MatrixBase<double>& result);
template MatrixBase<long double>& MatrixBase<long double>::Product(MatrixBase<long double>& m2, MatrixBase<long double>& result);