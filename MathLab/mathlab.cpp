// MathLab.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include "matrixbase.h"
#include "vector_functions.h"
#include "matrix_functions.h"
#include "instrumentation.h"
#include "thread_matrix.h"
using namespace MathLib;
using namespace std;

int main()
{
	//vectorPerformance();
	//ThreadMatrix<double> mat();
	matrixPerformance();
	Instrumentation::printResults();
}
