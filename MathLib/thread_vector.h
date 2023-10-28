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
	class ThreadVector : public MathVectorBase<T>
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
		ThreadVector(size_t size) :MathVectorBase<T>(size) {
			//std::cout << "ThreadVector(size_t size)" << std::endl;
			setNumThreads();
		}

		ThreadVector() :MathVectorBase<T>() {
			//std::cout << "ThreadVector()" << std::endl;
			setNumThreads();
		}

		ThreadVector(const std::initializer_list<T>& list):MathVectorBase<T>(list) {
			//std::cout << "ThreadVector(const std::initializer_list<T>& list)" << std::endl;
			setNumThreads();
		}
		~ThreadVector() {}

		T dotProduct(ThreadVector<T, MIN_SLICE_SIZE, MAX_THREADS>& v) {
			using namespace std;
			if (v.size != size) {
				throw MathException(VECTOR_SIZES_MUST_BE_THE_SAME);
			}

			// Split the array into num_thread slices
			//std::shared_ptr<T[]> result = std::make_shared<T[]>(num_threads);
			#if ORIG == 1
			std::shared_ptr<T[]> result;
			result = std::make_unique<T[]>(num_threads);
			#else
			T* result;
			result = new T[num_threads]();
			#endif	
			vector<std::thread> thd;
			size_t begin = 0;

			if (num_threads > 1) {
				size_t slice = size / num_threads;
				size_t end = begin + slice;
				for (size_t i = 0; i < num_threads - 1; ++i) {
					thd.push_back(std::thread(dpSlice, this, &v, begin, end, result, i));
					begin = end;
					end += slice;
				}
			}
			// Run the last slice in order
			dpSlice(this, &v, begin, size, result, num_threads - 1);

			if (num_threads > 1) {
				for (std::thread& t : thd) {
					if (t.joinable()) {
						t.join();
					}
				}
			}

			T sum = 0.0;
			//cout << "num_threads = " << num_threads << endl;
			for (size_t i = 0; i < num_threads; ++i) {
				sum += result[i];
			}
			//cout << "sum: " << sum << endl;

			#if ORIG == 0
			delete [] result;
			#endif
			return sum;
		}

	private:
	#if ORIG == 1
		static void dpSlice(ThreadVector<T, MIN_SLICE_SIZE, MAX_THREADS> *v1, 
							ThreadVector<T, MIN_SLICE_SIZE, MAX_THREADS> *v2, 
							size_t begin, size_t end, std::shared_ptr<T[]> const &result, size_t ndx) 
	#else
		static void dpSlice(ThreadVector<T, MIN_SLICE_SIZE, MAX_THREADS> *v1, 
						ThreadVector<T, MIN_SLICE_SIZE, MAX_THREADS> *v2, 
						size_t begin, size_t end, T *result, size_t ndx) 
	#endif
		{
			T sum = 0.0;
			for (size_t i = begin; i < end; ++i) {
				sum += v1->arr[i] * v2->arr[i];
			}
			//std::cout << "sum[" << ndx << "]=" << sum << std::endl;
			result[ndx] = sum;
		}
	};
};
