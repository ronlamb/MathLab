#include <iostream>
#include "matrixbase.h"
#include "messages.h"

using namespace MathLib;

template <typename T> void matrixProduct() {
	using namespace std;
	T m1 = {
		{ 1, 3, 1, 4, -1},
		{ 3, 9, -1, 5, 15},
		{ 0, 2, 1, -1, 2},
		{ 4, 0, -1, 1, -2}
	};

	cout << m1 << endl;
	T m2 = {
		{ 1, 2, 5, -2, 3, -1},
		{ 0, 1, 2, 1, 2, 2},
		{ 1, 0, 1, 1, 1, 1},
		{ 2, 1, 0, 1, 2, 2},
		{ 3, 2, 1, 0, 1, 2}
	};
	cout << line_separator << endl;
	cout << m2 << endl;
	/*
	 * 7, 7, 11, 6, 17, 12
	 * 57, 50, 47, 7, 51, 54
	 * 5, 5, 7, 2, 5, 7
	 * -1, 5, 17, -8, 11, -7
	 */
	T result(4, 6);
	cout << m1.Product(m2, result) << endl;
}
void matrixPerformance() {
	matrixProduct<MatrixBase<double>>();

	//vectorDotProduct<MathVectorBase<double>>();
	//vectorDotProduct<ThreadVector<double, 10>>();
	//vectorDotProduct<ThreadVectorLambda<double, 10>>();
	//vectorDotProduct<ThreadVectorOMP<double, 10>>();
	//size_t vectorSize = 100000000;
	//vectorTime<MathVectorBase<double>>(vectorSize, "vector");
	//vectorTime<ThreadVector<double, 10>>(vectorSize, "thdvector");
	//vectorTime<ThreadVectorLambda<double, 10>>(vectorSize, "lambdavector");
	//vectorTime<ThreadVectorOMP<double, 10>>(vectorSize, "ompvector");
}