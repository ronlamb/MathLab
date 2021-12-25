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
		T** row;			// Array of pointers to the first element of each row.
		void init();

	public:
		const T error_factor = std::numeric_limits<T>::min() * 100;

		MatrixBase() {}

		MatrixBase(size_t rows = 3, size_t columns = 0):rows(rows), columns(columns) {
			init();
		}

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
					row[i][j] = column;
					j++;
				}
				i++;
			}
		}

		~MatrixBase() {
			using namespace std;
		}

		void setCell(size_t i, size_t j, T value) {
			row[i][j] = value;
		}

		MatrixBase<T>& operator=(const MatrixBase<T>& right) {
			if (right.columns != columns || right.rows != rows) {
				throw MathException("Rows and columns don't match.");
			}
			for (int i = 0; i < right.rows; i++) {
				for (int j = 0; j < right.columns; j++) {
					row[i][j] = right.row[i][j];
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
						sum -= row[i][j] * b[j];
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
					sum -= row[i][j] * b[j];
				}
				b[i] = sum / row[i][i];
			}
		}
	};

	template <typename T> std::ostream& operator<<(std::ostream& os, const MatrixBase<T>& item) {
		using namespace std;
		os << "{ ";
		for (size_t i = 0; i < item.rows; ++i) {
			if (i) os << "  ";
			os << " {";
			for (size_t j = 0; j < item.columns; ++j) {
				if (j) os << ", ";
				os << item.row[i][j];
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
