// MathLab.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include "matrixbase.h"
#include "vector_functions.h"
#include "instrumentation.h"

using namespace MathLib;
using namespace std;

int main()
{
	MatrixBase<double> mat(3);
	simpleVectorDotProduct();
	vectorTime(1000);
	Instrumentation::printResults();
}
