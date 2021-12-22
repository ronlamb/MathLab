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

		void setNumThreads() {
			num_threads = NUM_THREADS;
			if (num_threads < 0) {
				throw MathException(NUM_THREADS_LT_0);
			}

			if (num_threads > std::thread::hardware_concurrency() || num_threads == 0) {
				num_threads = std::thread::hardware_concurrency();
				//std::cout << "Setting thread count to " << num_threads << std::endl;
			}

			if (size < num_threads * 10) {
				num_threads = 1;
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

		T dotProduct(ThreadVector<T, NUM_THREADS>& v) {
			using namespace std;
			if (v.size != size) {
				throw MathException(VECTOR_SIZES_MUST_BE_THE_SAME);
			}

			// Split the array into num_thread slices
			std::shared_ptr<T[]> result = std::make_shared<T[]>(num_threads);

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

			return sum;
		}

	private:
		static void dpSlice(ThreadVector<T, NUM_THREADS> *v1, ThreadVector<T, NUM_THREADS> *v2, size_t begin, size_t end, std::shared_ptr<T[]> const &result, size_t ndx) {
			T sum = 0.0;
			for (size_t i = begin; i < end; ++i) {
				sum+= v1->arr[i] * v2->arr[i];
			}
			//std::cout << "sum[" << ndx << "]=" << sum << std::endl;
			result[ndx] = sum;
		}
	};
};
