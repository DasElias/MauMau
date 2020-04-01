#include "InvalidInputException.h"

namespace card {
	InvalidInputException::InvalidInputException(int statusCode) throw() :
			std::runtime_error(""),
			statusCode(statusCode) {
	}
	char const* InvalidInputException::what() const throw() {
		return exception::what();
	}
	int InvalidInputException::getStatusCode() const throw() {
		return statusCode;
	}
}