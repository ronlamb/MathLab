#pragma once
#include <initializer_list>
#include <iostream>
#include <ostream>
#include "mathexception.h"

namespace MathLib {
	template <typename T>
	class MathVectorBase
	{
	protected:
		size_t size;
		T* arr;

	public:
		MathVectorBase() :size(3), arr(new T[3]{}) {}

		// Base constructor
		MathVectorBase(size_t size) :size(size), arr(new T[size]{}) {}

		// List initializer
		MathVectorBase(const std::initializer_list<T>& list):MathVectorBase(list.size()) {
			size_t i = 0;
			for (auto element : list) {
				arr[i++] = element;
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

		T& operator[](size_t i) {
			return arr[i];
		}

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
		template<typename U> friend std::ostream& operator<<(std::ostream& os, const MathVectorBase<U>& item);
	};

	template <typename T> std::ostream& operator<<(std::ostream& os, const MathVectorBase<T>& item) {
		using namespace std;
		os << "{ ";
		for (size_t i = 0; i < item.size; ++i) {
			if (i) os << ", ";
			os << item.arr[i];
			if (i && i % 8 == 0) os << endl;
		}
		os << " }";

		return os;
	}
}

