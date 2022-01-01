//#include <iostream>
//#include <vector>
//#include <string>
#include "mathvectorbase.h"
#include "thread_vector.h"
#include "thread_vector_lambda.h"
#include "thread_vector_omp.h"
#include "instrumentation.h"
#include "messages.h"

using namespace MathLib;
using namespace std;

template <typename T> void vectorDotProduct() {
	T v1 = { 1.0, 2.0, 3.0, 4.0, -2, -7, -0.11, 1, 0, 0,
				  0, -12, 1, 2, 3, -5, 3, 1, 2, 1, 7, .12, .93 };
	T v2 = { 0.75, .0125, .175, .0675, .5, -.1675, 1.111, .1, 120, 120,
			 0, 1.22, .21, .72, 3, 2, -124, 1, 1, 1, -1, -1, 12 };
	double expected = -376.22971;
	const auto precision = std::cout.precision();
	cout.precision(3);
	cout << fixed
		<< line_separator << endl
		<< v1 << endl << " . " << v2 << " = "
		<< std::setprecision(5) << v1.dotProduct(v2)
		<< ", expected=" << expected << endl
		<< line_separator << endl << endl;
	cout.precision(precision);
}

template <typename T> void vectorTime(size_t size, string message) {
	T v1(size);
	T v2(size);

	for (size_t i = 0; i < size; ++i) {
		v1[i] = i * .1372;
		v2[i] = v1[i] - (i % 100) / 113.0;
	}

	string title = message + " " + to_string(size);
	for (size_t i = 0; i < 100; ++i) {
		INSTR_NAME_START(tmp, title);
		v1.dotProduct(v2);
		INSTR_NAME_END(tmp);
	}
}

void vectorPerformance() {
	vectorDotProduct<MathVectorBase<double>>();
	vectorDotProduct<ThreadVector<double, 10>>();
	vectorDotProduct<ThreadVectorLambda<double, 10>>();
	vectorDotProduct<ThreadVectorOMP<double, 10>>();
	size_t vectorSize = 100000000;
	vectorTime<MathVectorBase<double>>(vectorSize, "vector");
	vectorTime<ThreadVector<double, 10>>(vectorSize,"thdvector");
	vectorTime<ThreadVectorLambda<double, 10>>(vectorSize, "lambdavector");
	vectorTime<ThreadVectorOMP<double, 10>>(vectorSize, "ompvector");
}