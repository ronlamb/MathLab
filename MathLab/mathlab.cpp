// MathLab.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include "matrixbase.h"
#include "mathvectorbase.h"

int main()
{
	using namespace MathLib;
	using namespace std;
	MatrixBase<double> mat(3);
	MathVectorBase<double> vect = { 1.0, 2.0, 3.0, 4.0 };

	cout << "Vector: " << vect << endl;
}
