#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <regex>
#include <filesystem>
#include "matrixbase.h"
#include "util.h"
#include "yaml-cpp/yaml.h"

using namespace MathLib;

template <typename T>
class TestMatrix
{
private:
	size_t lineno;

	void runTest(MatrixBase<T>& m, const YAML::Node &test) {
		using namespace std;
		cout << "Testing " << m.getRows() << "x" << m.getColumns() << " matrix: " << test["name"] << "\n";
		string matrix = test["matrix"].as<string>();
		T determinate = test["determinate"].as<T>();

		size_t columns = m.getRows();
		size_t rows = m.getRows();
		size_t cells_remain = rows * columns;
		size_t row = 0;
		size_t column = 0;
		T onevalue;
		stringstream ss(matrix);
		while (ss >> onevalue && cells_remain) {
			m[row][column] = onevalue;
			column++;
			if (column == columns) {
				column = 0;
				row++;
			}
			cells_remain--;
		}
		cout << "matrix read:\n" << m << endl;
		cout << "  determinate = " << fixed << setprecision(8) << m.determinate() << " expected: " << m.determinate() << endl;
	}
public:
	TestMatrix(const std::string filename) {
		using namespace std;
		try {
			YAML::Node test = YAML::LoadFile(filename);
			YAML::Node size = test["TestFile"]["size"];
			YAML::Node tests = test["TestFile"]["tests"];
			size_t rows = size["rows"].as<size_t>();
			size_t columns = size["columns"].as<size_t>();
			//cout << "size = " << size << "\n"
			//	<< "tests:\n" << tests << endl;
			MatrixBase<T> m(rows, columns);
			for (YAML::const_iterator it = tests.begin(); it != tests.end(); ++it) {
				runTest(m, it->as<YAML::Node>());
			}
		}
		catch (YAML::ParserException e) {
			cout << "Parse exception " << e.what() << endl;
		}
	}
};

