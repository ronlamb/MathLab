#pragma once
#include "mathvectorbase.h"
#include "mathexception.h"
#include <thread>
namespace MathLib {
	template <typename T, size_t NUM_THREADS = 0>
	class ThreadVector : public MathVectorBase<T>
	{
	protected:
		using MathVectorBase<T>::size;
		using MathVectorBase<T>::arr;
		size_t num_threads;
	public:
		ThreadVector(size_t size) :MathVectorBase<T>(size) {
			num_threads = NUM_THREADS;
			if (num_threads < 0) {
				throw MathException(NUM_THREADS_LT_0);
			}

			if (num_threads > std::thread::hardware_concurrency() || num_threads == 0) {
				num_threads = std::thread::hardware_concurrency();
			}
		}
		ThreadVector() :MathVectorBase<T>() {
			num_threads = NUM_THREADS;
			if (num_threads < 0) {
				throw MathException(NUM_THREADS_LT_0);
			}

			if (num_threads > std::thread::hardware_concurrency() || num_threads == 0) {
				num_threads = std::thread::hardware_concurrency();
			} 
		}

		ThreadVector(const std::initializer_list<T>& list):MathVectorBase(list) {}
		~ThreadVector() {}

		T dotProduct(ThreadVector<T>& v) {
			using namespace std;
			if (v.size != size) {
				throw MathException(VECTOR_SIZES_MUST_BE_THE_SAME);
			}

			// Split the array into num_thread slices
			T *result = new T[num_threads];
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
			for (size_t i = 0; i < num_threads; ++i) {
				sum += result[i];
			}
			delete[] result;
			return sum;
		}

	private:
		static void dpSlice(ThreadVector<T> *v1, ThreadVector<T> *v2, size_t begin, size_t end, T *result, size_t ndx) {
			T sum = 0.0;
			for (size_t i = begin; i < end; ++i) {
				sum+= v1->arr[i] + v2->arr[i];
			}

			result[ndx] = sum;
		}
	};
};
