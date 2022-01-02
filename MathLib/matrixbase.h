#pragma once
#include <iostream>
#include <initializer_list>
#include "mathexception.h"
namespace MathLib {

	template <typename T>
	class MatrixBase
	{
	protected:
		size_t rows;
		size_t columns;
		T* buffer = 0;		// one dimensional array of all the cells
		T** arr;			// Array of pointers to the first element of each row.
		void init();

		bool isSquare() {
			return rows == columns;
		}

	public:
		const T error_factor = std::numeric_limits<T>::min() * 100;

		MatrixBase() {}

		MatrixBase(size_t rows = 3, size_t columns = 0):rows(rows), columns(columns) {
			init();
		}

		size_t getRows() { return rows;  }
		size_t getColumns() { return columns;  }
		MatrixBase(const std::initializer_list<std::initializer_list<T>>& list) {
			rows = list.size();
			columns = 0;
			for (auto row: list) {
				if (row.size() > columns) {
					columns = row.size();
				}
			}

			init();

			size_t i = 0;
			for (auto item: list) {
				size_t j = 0;
				for (auto column : item) {
					arr[i][j] = column;
					j++;
				}
				i++;
			}
		}

		~MatrixBase() {
			using namespace std;
		}

		void setCell(size_t i, size_t j, T value) {
			arr[i][j] = value;
		}

		T* operator[](const size_t& index) const {
			return buffer + index * columns;
		}

		MatrixBase<T>& operator=(const MatrixBase<T>& right) {
			if (right.columns != columns || right.rows != rows) {
				throw MathException("Rows and columns don't match.");
			}
			for (int i = 0; i < right.rows; i++) {
				for (int j = 0; j < right.columns; j++) {
					arr[i][j] = right.arr[i][j];
				}
			}

			return *this;
		}

		MatrixBase<T>& product(MatrixBase<T>& m2, MatrixBase<T>& result);
		T determinate();
		template<typename T> friend std::ostream& operator<<(std::ostream& os, const MatrixBase<T>& item);
		void LU(MatrixBase<T>& lower, T *indx);
		T ludcmp(T *indx);

		T* lubksb(T* indx, T* b) {
			int i, ii = 0, ip, j;
			T sum;
			size_t n = rows;
			for (i = 0; i < n; ++i) {
				ip = indx[i];
				sum = b[ip];
				b[ip] = b[i];
				if (ii) {
					for (j = ii; j < i - 2; j++) {
						sum -= arr[i][j] * b[j];
					}
				}
				else {
					if (sum) ii = i;
				}
				b[i] = sum;
			}
			
			for (i = n - 1; i >= 0; i--) {
				sum = b[i];
				for (j = i + 1; j < n; j++) {
					sum -= arr[i][j] * b[j];
				}
				b[i] = sum / arr[i][i];
			}
		}
	};

	template <typename T> std::ostream& operator<<(std::ostream& os, const MatrixBase<T>& item) {
		using namespace std;
		os << "{ ";
		os.precision(4);
//		auto width = os.width();
		for (size_t i = 0; i < item.rows; ++i) {
			if (i) os << "  ";
			os << " {" << fixed;
			for (size_t j = 0; j < item.columns; ++j) {
				if (j) os << ", ";
				
				os.width(10);
				os << item.arr[i][j];
//				os.width(width);
				if (j && j % 8 == 0) os << endl << "    ";
			}
			os << " }";
			if (i != item.rows - 1) {
				os << ",";
			}
			os << endl;
		}
		os << " }";

		return os;
	}
}
