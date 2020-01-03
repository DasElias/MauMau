#pragma once
#include <stdexcept>

namespace card {
	class PlayerNotFoundException : public std::runtime_error {
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			PlayerNotFoundException(char const* const message) throw();
			PlayerNotFoundException(std::string message) throw();
			
		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:	
			virtual char const* what() const throw();
	};
}