#include <iostream>
#include "matrixbase.h"
#include "messages.h"
#include "test_matrix.h"
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

	T result(4, 6);
	cout << m1.product(m2, result) << endl;

	cout << "first 3 diag " << m1[0][0] << ", " << m1[1][1] << ", " << m1[2][2] << endl;

}
template <typename T, typename Y> void ludcmp() {
	T mat = {
		{ 1, 2, 5, -2, 3, -1},
		{ 0, 1, 2, 1, 2, 2},
		{ 1, 0, 1, 1, 1, 1},
		{ 2, 1, 0, 1, 2, 2},
		{ 3, 2, 1, 0, 1, 2},
		{ 1, 2, 3, -1, -2 ,-3}
	};

	using namespace std;
	T dcmp(mat);

	cout << "dcmp:\n" << dcmp << endl;
	cout << "Det = " << mat.determinate() << endl;
	Y indx[6];
	cout << "Det = " << dcmp.ludcmp(indx) << endl;
	cout << "After:\n" << dcmp << endl;
}
void matrixPerformance() {
	//matrixProduct<MatrixBase<double>>();
	//ludcmp<MatrixBase<double>, double>();
	TestMatrix<double> test1("testmatrixes/six_by_six_1.yaml");
}