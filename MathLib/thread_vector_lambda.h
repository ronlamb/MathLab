#pragma once
/* Enhancement to ThreadVector to run the threads in a lambda instead of in 
 * a separate static function.
 *
 * The template takes as parameters:
 *	typename T				The type (double, long double)
 *	size_t MIN_SLICE_SIZE	The minimum slice size, defaults to 10
 *	size_t MAX_THREADS		The maximum number of threads
 *
 * It will calculate the number of threads needed to run with at least the minimum
 * slice size.  The class will spawn up to the calculated number of threads - 1
 * threads.
 *
 * If the size is less than MIN_SLICE_SIZE then it will only run in the current
 * thread, and not spawn any more threads.
 */
#include "mathvectorbase.h"
#include "mathexception.h"
#include <thread>
namespace MathLib {
	template <typename T, size_t MIN_SLICE_SIZE = 10, size_t MAX_THREADS = 0>
	class ThreadVectorLambda : public MathVectorBase<T>
	{
	protected:
		using MathVectorBase<T>::size;
		using MathVectorBase<T>::arr;
		size_t num_threads;
		size_t slice_size = 0;

		void setNumThreads() {
			num_threads = MAX_THREADS;
			if (num_threads < 0) {
				throw MathException(NUM_THREADS_LT_0);
			}

			if (num_threads > std::thread::hardware_concurrency() || num_threads == 0) {
				num_threads = std::thread::hardware_concurrency();
			}

			size_t num_slices = size / MIN_SLICE_SIZE;
			if (num_slices < 0) {
				num_threads = 1;
			}
			else if (num_slices < num_threads) {
				num_threads = num_slices;
			}

			if (num_threads > 1) {
				slice_size = size / num_threads;
			}
		}
	public:
		ThreadVectorLambda(size_t size) :MathVectorBase<T>(size) {
			setNumThreads();
		}

		ThreadVectorLambda() :MathVectorBase<T>() {
			setNumThreads();
		}

		ThreadVectorLambda(const std::initializer_list<T>& list) :MathVectorBase<T>(list) {
			setNumThreads();
		}
		~ThreadVectorLambda() {}

		T dotProduct(ThreadVectorLambda<T, MIN_SLICE_SIZE, MAX_THREADS>& v) {
			using namespace std;
			if (v.size != size) {
				throw MathException(VECTOR_SIZES_MUST_BE_THE_SAME);
			}

			// Split the array into num_thread slices
			//std::shared_ptr<T[]> result = std::make_shared<T[]>(num_threads);
			std::shared_ptr<T[]> result;
			result = std::make_unique<T[]>(num_threads);
			
			vector<std::thread> thd;
			size_t begin = 0;

			if (num_threads > 1) {
				size_t end = begin + slice_size;
				T* arr2 = v.arr;

				for (size_t i = 0; i < num_threads - 1; ++i) {
					thd.push_back(std::thread([=,this,&result, &arr2]() {
						T sum = 0.0;
						for (size_t j = begin; j < end; ++j) {
							sum += arr[j] * arr2[j];
						}
						result[i] = sum;
					}));
					begin = end;
					end += slice_size;
				}
			}

			// Run the last slice in the current thread
			T sum = 0.0;
			for (size_t j = begin; j < size; ++j) {
				sum += arr[j] * v.arr[j];
			}
			//result[num_threads - 1] = sum;

			//std::cout << "range " << begin << " to " << size << " sum=" << sum << std::endl;

			if (num_threads > 1) {
				for (std::thread& t : thd) {
					if (t.joinable()) {
						t.join();
					}
				}
				for (size_t i = 0; i < num_threads -1; ++i) {
					sum += result[i];
				}
			}

			// Sum up the slice results
			//sum = 0.0;
			// for (size_t i = 0; i < num_threads -1; ++i) {
			// 	sum += result[i];
			// }

			return sum;
		}
	};
};
