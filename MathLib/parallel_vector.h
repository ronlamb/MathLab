#pragma once
/* Initial threaded version of the Vector class.
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
#include <memory>
#include <vector>

#define ORIG 0
namespace MathLib {
	template <typename T, size_t MIN_SLICE_SIZE=10, size_t MAX_THREADS=0>
	class ParallelVector : public MathVectorBase<T>
	{
	protected:
		using MathVectorBase<T>::size;
		using MathVectorBase<T>::arr;
		size_t num_threads;

		void setNumThreads() {
			num_threads = MAX_THREADS;
			if (num_threads < 0) {
				throw MathException(NUM_THREADS_LT_0);
			}

			if (num_threads > std::thread::hardware_concurrency() || num_threads == 0) {
				num_threads = std::thread::hardware_concurrency();
				//std::cout << "Setting thread count to " << num_threads << std::endl;
			}

			size_t num_slices = size / MIN_SLICE_SIZE;
			if (num_slices < 0) {
				num_threads = 1;
			}
			else if (num_slices < num_threads) {
				num_threads = num_slices;
			}
		}
	public:
		ParallelVector(size_t size) :MathVectorBase<T>(size) {
			//std::cout << "ThreadVector(size_t size)" << std::endl;
			setNumThreads();
		}

		ParallelVector() :MathVectorBase<T>() {
			//std::cout << "ThreadVector()" << std::endl;
			setNumThreads();
		}

		ParallelVector(const std::initializer_list<T>& list):MathVectorBase<T>(list) {
			//std::cout << "ThreadVector(const std::initializer_list<T>& list)" << std::endl;
			setNumThreads();
		}
		~ParallelVector() {}

		T dotProduct(ParallelVector<T, MIN_SLICE_SIZE, MAX_THREADS>& v) {
			using namespace std;
			if (v.size != size) {
				throw MathException(VECTOR_SIZES_MUST_BE_THE_SAME);
			}

			# Initial try of accumulate
            T sum = std::accumulate(begin, end, 0.0, [this] (size_t i)
            {
                return arr[i]* v.arr[i];
            }
            );
''			return sum;
		}

	
};