#include <iostream>
#include <vector>
#include "mathvectorbase.h"

using namespace MathLib;

void simpleVectorDotProduct() {
	using namespace std;
	MathVectorBase<double> v1 = { 1.0, 2.0, 3.0, 4.0 };
	MathVectorBase<double> v2 = { 0.75, .0125, .175, .0675 };
	double expected = 1.57;
	const auto precision = std::cout.precision();
	cout.precision(3);
	cout << fixed 
		<< "-----------------------------------------------------------------------------------" << endl
		<< v1 << " . " << v2 << " = " << v1.dotProduct(v2) << endl
		<< "expected  = " << expected << endl
		<< "-----------------------------------------------------------------------------------" << endl;
	cout.precision(precision);
}