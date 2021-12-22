#include <iostream>
#include <vector>
#include <string>
#include "mathvectorbase.h"
#include "thread_vector.h"
#include "instrumentation.h"
#include "messages.h"

using namespace MathLib;
using namespace std;

void simpleVectorDotProduct() {
	MathVectorBase<double> v1 = { 1.0, 2.0, 3.0, 4.0 };
	MathVectorBase<double> v2 = { 0.75, .0125, .175, .0675 };
	double expected = 1.57;
	const auto precision = std::cout.precision();
	cout.precision(3);
	cout << fixed 
		<< line_separator << endl
		<< v1 << " . " << v2 << " = " << v1.dotProduct(v2)
		<< ", expected=" << expected << endl
		<< line_separator << endl << endl;
	cout.precision(precision);
}

void thdVectorDotProduct() {
	ThreadVector<double, 2> v1 = { 1.0, 2.0, 3.0, 4.0, -2, -7, -0.11, 1, 0, 0,
		                           0, -12, 1, 2, 3, -5, 3, 1, 2, 1, 7, .12, .93 };
	ThreadVector<double, 2> v2 = { 0.75, .0125, .175, .0675, .5, -.1675, 1.111, .1, 120, 120,
								   0, 1.22, .21, .72, 3, 2, -124, 1, 1, 1, -1, -1, 12
								  };
	double expected = 1.57;
	const auto precision = std::cout.precision();
	cout.precision(3);
	cout << fixed
		<< line_separator << endl
		<< v1 << " . " << v2 << " = " << v1.dotProduct(v2)
		<< ", expected=" << expected << endl
		<< line_separator << endl << endl;
	cout.precision(precision);
}


void vectorTime(size_t size) {
	MathVectorBase<double> v1(size);
	MathVectorBase<double> v2(size);

	for (size_t i = 0; i < size; ++i) {
		v1[i] = i * .1372;
		v2[i] = v1[i] - (i % 100) / 113.0;
	}

	string title = "vector " + to_string(size);
	for (size_t i = 0; i < 100; ++i) {
		INSTR_NAME_START(tmp, title);
		v1.dotProduct(v2);
		INSTR_NAME_END(tmp);
	}
}

void threadVectorTime(size_t size) {
	ThreadVector<double> v1(size);
	ThreadVector<double> v2(size);

	for (size_t i = 0; i < size; ++i) {
		v1[i] = i * .1372;
		v2[i] = v1[i] - (i % 100) / 113.0;
	}

	string title = "thdvector " + to_string(size);
	for (size_t i = 0; i < 100; ++i) {
		INSTR_NAME_START(tmp, title);
		v1.dotProduct(v2);
		INSTR_NAME_END(tmp);
	}

}