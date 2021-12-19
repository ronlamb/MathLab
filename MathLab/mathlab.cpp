// MathLab.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include "matrixbase.h"
#include "vector_functions.h"

int main()
{
	using namespace MathLib;
	using namespace std;
	MatrixBase<double> mat(3);
	simpleVectorDotProduct();
}
