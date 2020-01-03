#pragma once
#include <string>

namespace card {
	class NetworkErrorHandler {
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		protected:
			NetworkErrorHandler() = default;

		public:
			virtual ~NetworkErrorHandler() = default;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual void resetAndShowError(std::string errorMessage, std::string title) =0;
	};
}