#include "RegistryException.h"

namespace card {
	RegistryException::RegistryException(char const* const message) throw() :
			std::runtime_error(message) {
	}
	RegistryException::RegistryException(std::string message) throw() :
			std::runtime_error(message.c_str()) {
	}
	char const* RegistryException::what() const throw() {
		return exception::what();
	}
}