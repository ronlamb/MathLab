#pragma once
#include <iostream>
namespace MathLib {

	template <typename T>
	class MatrixBase
	{
	protected:
		size_t rows;
		size_t columns;

	public:
		MatrixBase(size_t rows = 3, size_t columns = 0);
		~MatrixBase() {
			using namespace std;
			cout << "Destroying MatrixBase object." << endl;
		}
	};
}
