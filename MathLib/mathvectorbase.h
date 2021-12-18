#pragma once
#include <initializer_list>
#include <iostream>
#include <ostream>
#include "mathexception.h"
namespace MathLib {
	template <typename T>
	class MathVectorBase
	{
	private:
		T* arr;
		size_t size;

	public:
		// Base constructor
		MathVectorBase(size_t size) :size(size), arr(new T[size]{}) {}

		// List initializer
		MathVectorBase(const std::initializer_list<T>& list):MathVectorBase(list.size()) {
			size_t i = 0;
			for (auto element : list) {
				arr[i] = element;
				++i;
			}
		}

		~MathVectorBase() {
			if (arr) {
				delete [] arr;
			}
		}

		//MathVectorBase<T>& operator=(const vector<T> &vect) {
		//	return MathVectorBase<T>(&vect);
		//}

		T dotProduct(MathVectorBase<T>& v) {
			using namespace std;
			if (v.size != size) {
				throw MathException(VECTOR_SIZES_MUST_BE_THE_SAME);
			}
			T result = 0.0;

			for (size_t i = 0; i < size; ++i) {
				result += arr[i] * v.arr[i];
			}

			return result;
		}

		MathVectorBase<T>& crossProduct(MathVectorBase<T>& v, MathVectorBase<T>& result) {
			using namespace std;
			if (v.size != 3 || size != 3 || result.size != 3) {
				throw MathException(CROSS_PRODUCTS_NOT_3D);
			}

			result.arr[0] = (arr[1] * v.arr[2]) - (arr[2] * v.arr[1]);
			result.arr[1] = -(arr[0] * v.arr[2]) - (arr[2] * v.arr[0]);
			result.arr[2] = (arr[0] * v.arr[1]) - (arr[1] * v.arr[0]);

			return result;
		}
		friend std::ostream& operator<<(std::ostream& os, const MathVectorBase& item);
	};


	template <typename T> std::ostream& operator<<(std::ostream& os, const MathVectorBase<T>& item) {
		using MathVectorBase<T>::arr;
		using MathVectorBase<T>::size;
		using namespace std;
		os << "{ ";
		for (size_t i = 0; i < size; ++i) {
			if (i) os << ", ";
			os << arr[i];
		}
		os << " }";

		return os;
	}

}

