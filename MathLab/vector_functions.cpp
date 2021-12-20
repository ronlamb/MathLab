#include <iostream>
#include <vector>
#include "mathvectorbase.h"
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
		<< v1 << " . " << v2 << " = " << v1.dotProduct(v2) << endl
		<< "expected  = " << expected << endl
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

	for (size_t i = 0; i < 100; ++i) {
		INSTR_NAME_START(tmp, "LU 1000x1000");
		v1.dotProduct(v2);
		INSTR_NAME_END(tmp);
	}
}