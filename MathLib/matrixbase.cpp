#include "pch.h"
#include "matrixbase.h"

//using namespace MathLib;

namespace MathLib {
template <typename T> MatrixBase<T>::MatrixBase(size_t rows, size_t columns) {
	using namespace std;
	if (columns == 0) {
		columns = rows;
	}
	this->rows = rows;
	this->columns = columns;
	cout << "Created MatrixBase class" << endl;
}

template MatrixBase<double>::MatrixBase(size_t rows, size_t columns);
template MatrixBase<long double>::MatrixBase(size_t rows, size_t columns);
}