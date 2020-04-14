#pragma once
#include <stdexcept>

namespace card {
	class RegistryException : public std::runtime_error {
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			RegistryException(char const* const message) throw();
			RegistryException(std::string message) throw();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual char const* what() const throw();
	};
}