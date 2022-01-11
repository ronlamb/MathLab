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

	void loadMatrix(MatrixBase<T>& m, size_t rows, size_t columns, const std::string& values) {
		using namespace std;
		stringstream ss(values);
		size_t cells_remain = rows * columns;
		size_t row = 0;
		size_t column = 0;
		T value;
		while (ss >> value && cells_remain) {
			m[row][column] = value;
			column++;
			if (column == columns) {
				column = 0;
				row++;
			}
			cells_remain--;
		}
	}
	void testDeterminate(const YAML::Node& test) {
		using namespace std;
		size_t rows = test["matrix"]["size"]["rows"].as<size_t>();
		size_t columns = test["matrix"]["size"]["columns"].as<size_t>();
		T determinate = test["determinate"].as<T>();
		cout << "Test " << test["name"] << ": " << rows << "x" << columns << " matrix\n";

		MatrixBase<T> m(rows, columns);
		loadMatrix(m, rows, columns, test["matrix"]["values"].as<string>());
		cout << "matrix read:\n" << m << endl;
		cout << "  determinate = " << fixed << setprecision(8) << m.determinate() << " expected: " << determinate << endl;
	}

	void testProduct(const YAML::Node& test) {
		using namespace std;
		size_t rows = test["matrix1"]["size"]["rows"].as<size_t>();
		size_t columns = test["matrix1"]["size"]["columns"].as<size_t>();
		MatrixBase<T> m1(rows, columns);
		loadMatrix(m1, rows, columns, test["matrix1"]["values"].as<string>());

		rows = test["matrix2"]["size"]["rows"].as<size_t>();
		columns = test["matrix2"]["size"]["columns"].as<size_t>();
		MatrixBase<T> m2(rows, columns);
		loadMatrix(m2, rows, columns, test["matrix2"]["values"].as<string>());
	}
public:
	TestMatrix(const std::string filename) {
		using namespace std;
		try {
			YAML::Node test = YAML::LoadFile(filename);
			YAML::Node tests = test["TestFile"]["tests"];
			
			for (YAML::const_iterator it = tests.begin(); it != tests.end(); ++it) {
				YAML::Node node = it->as<YAML::Node>();
				if (node["type"].as<string>() == "determinate") {
					testDeterminate(node);
				}
				if (node["type"].as<string>() == "product") {
					testProduct(node);
				}
			}
		}
		catch (YAML::ParserException e) {
			cout << "Parse exception " << e.what() << endl;
		}
	}
};

