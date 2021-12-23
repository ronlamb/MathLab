#pragma once
#include <iostream>
#include <initializer_list>
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
		MatrixBase() {}

		MatrixBase(size_t rows = 3, size_t columns = 0):rows(rows), columns(columns) {
			init();
		}

		MatrixBase(const std::initializer_list<std::initializer_list<T>>& list) {//: MatrixBase<T>(list.size(), list[0].size()) {
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

		MatrixBase<T>& Product(MatrixBase<T>& m2, MatrixBase<T>& result);

		template<typename T> friend std::ostream& operator<<(std::ostream& os, const MatrixBase<T>& item);
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
