#pragma once
#include <thread>
#include "matrixbase.h"

namespace MathLib {
	template <typename T, size_t MIN_SLICE_SIZE = 10, size_t MAX_THREADS = 0>
	class ThreadMatrix : MatrixBase<T>
	{
	private:
		using MatrixBase<T>::rows;
		using MatrixBase<T>::columns;
		using MatrixBase<T>::buffer;
		using MatrixBase<T>::arr;
		size_t num_threads;
		size_t size {};

		void setNumThreads() {
			num_threads = MAX_THREADS;
			if (num_threads < 0) {
				throw MathException(NUM_THREADS_LT_0);
			}

			if (num_threads > std::thread::hardware_concurrency() || num_threads == 0) {
				num_threads = std::thread::hardware_concurrency();
				//std::cout << "Setting thread count to " << num_threads << std::endl;
			}

			//size_t num_slices = size / MIN_SLICE_SIZE;
			//if (num_slices < 0) {
			//	num_threads = 1;
			//}
			//else if (num_slices < num_threads) {
			//	num_threads = num_slices;
			//}
		}
	public:
		ThreadMatrix():MatrixBase<T>() {
			size = rows * columns;
			setNumThreads();
		}
		ThreadMatrix(size_t rows, size_t columns = 0): MatrixBase<T>(rows, columns) {
			size = rows * columns;
			setNumThreads();
		}

		ThreadMatrix(const std::initializer_list<std::initializer_list<T>>& list) : MatrixBase<T>(list) {
			size = rows * columns;
			setNumThreads();
		}

		~ThreadMatrix() {
			using namespace std;
		}

		ThreadMatrix& product(ThreadMatrix& m2, ThreadMatrix& result) {
			using namespace std;
			if (columns != m2.rows) {
				throw MathException(COLUMNS_MUST_MATCH_ROWS);
			}

			vector<std::thread> thd;
			size_t begin = 0;

			if (num_threads > 1) {
				// example for 100x90 * 90x80 = 100x80 result table with 23 threads and min size = 100 in 24 threads
				// result_size = 100 * 80 = 8000
				// cells_per_slice = 8000 / 24 = 333
				// slice = 333 / 100 = 3 + 1
				// 24 * 4 = 96 < 100 -> slice = 4
				// remain =100 - 96 = 4
				// mod = 24 / 4 = 6

				size_t result_size = rows * m2.columns;
				size_t cells_per_slice = result_size / num_threads;
				size_t slice = cells_per_slice / rows;
				if (num_threads * (slice + 1) < rows) {
					slice++;
				}
				size_t remain = rows - slice * num_threads;
				size_t mod = num_threads / remain;

				size_t end = begin + slice;
				for (size_t i = 0; i < num_threads - 1; ++i) {
					thd.push_back(std::thread(prodSlice, this, &m2, columns, begin, end, result, i));
					begin = end;
					end += slice;
					if (i % mod == 0) { end++; } // placing at end means for 24 threads 1, 7, 13, 19 get updated 
				}
			}

			prodSlice(this, &m2, columns, begin, size, result, num_threads - 1);

			cout << "result size = " << result.rows << "x" << result.columns << endl;
			if (rows != result.rows || m2.columns != result.columns) {
				throw MathException("Result must be an " + to_string(rows) + " by" + to_string(m2.columns) + " matrix.");
			}

			for (size_t i = 0; i < rows; i++) {
				for (size_t j = 0; j < m2.columns; j++) {
					T sum = 0.0;
					for (size_t k = 0; k < columns; k++) {
						sum += arr[i][k] * m2.arr[k][j];
					}
					result.arr[i][j] = sum;
				}
			}

			return result;
		}

	private:
		static void prodSlice(ThreadMatrix<T, MIN_SLICE_SIZE, MAX_THREADS>* m1,
			ThreadMatrix<T, MIN_SLICE_SIZE, MAX_THREADS>* m2,
			ThreadMatrix<T, MIN_SLICE_SIZE, MAX_THREADS>* result,
			size_t columns, size_t row_begin, size_t row_end
		)
		{
			for (size_t i = row_begin; i < row_end; ++i) {
				for (size_t j = 0; j < m2->columns; ++j) {
					T sum = 0.0;
					for (size_t k = 0; k < columns; ++k) {
						sum += m1->arr[i][k] * m2->arr[k][j];
					}
					result->arr[i][j] = sum;
				}
			}
		}
	};
}
