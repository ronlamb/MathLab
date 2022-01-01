#include "pch.h"
#include <string>
#include <exception>
#include "mathexception.h"
namespace MathLib {
	const std::string COLUMNS_MUST_MATCH_ROWS = "Column size in left matrix must match rows in right matrix";
	const std::string MUST_BE_A_SQUARE = "Matrix must be a square matrix";
	const std::string VECTOR_SIZES_MUST_BE_THE_SAME = "Vector sizes must be the same for operation.";
	const std::string CROSS_PRODUCTS_NOT_3D = "Cross products are only defined in 3 dimensional space.";
	const std::string NUM_THREADS_LT_0 = "Number of threads must be greater than or equal to zero.";
	const std::string SINGULAR_MATRIX = "Singular matrix in LUDCMP";
};