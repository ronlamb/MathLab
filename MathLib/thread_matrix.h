#pragma once
#include "matrixbase.h"

namespace MathLib {
	template <typename T, size_t MIN_SLICE_SIZE = 10, size_t MAX_THREADS = 0>
	class ThreadMatrix : MatrixBase<T>
	{
	private:
		using MatrixBase<T>::rows;
		using MatrixBase<T>::columns;
		using MatrixBase<T>::buffer;
		using MatrixBase<T>::row;
		size_t num_threads;

	public:
		ThreadMatrix():MatrixBase<T>() {}
		ThreadMatrix(size_t rows = 3, size_t columns = 0): MatrixBase<T>(rows, columns) {}

		ThreadMatrix(const std::initializer_list<std::initializer_list<T>>& list) : MatrixBase<T>(list) {}

		~ThreadMatrix() {
			using namespace std;
		}

		ThreadMatrix& Product(ThreadMatrix& m2, ThreadMatrix& result) {
			return result;
		}


	};
}
