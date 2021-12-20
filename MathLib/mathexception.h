#pragma once
#include <string>
#include <stdexcept>
namespace MathLib {
	const extern std::string COLUMNS_MUST_MATCH_ROWS;
	const extern std::string MUST_BE_A_SQUARE;
	const extern std::string VECTOR_SIZES_MUST_BE_THE_SAME;
	const extern std::string CROSS_PRODUCTS_NOT_3D;

	class MathException : public std::exception {
	protected:
		std::string message;

	public:
		MathException(const std::string& message) : message(message) {}

		virtual const char* what() const noexcept override
		{
			return message.c_str();
		}
	};
}

