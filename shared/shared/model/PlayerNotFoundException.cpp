#include "PlayerNotFoundException.h"

namespace card {
	PlayerNotFoundException::PlayerNotFoundException(char const* const message) throw() :
			std::runtime_error(message) {
	
	}

	PlayerNotFoundException::PlayerNotFoundException(std::string message) throw() :
			PlayerNotFoundException(message.c_str()) {
	}

	char const* PlayerNotFoundException::what() const throw() {
		return exception::what();
	}
}