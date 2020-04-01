#pragma once
#include <string>

namespace card {
	class AbstractConnectionToClient {
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			AbstractConnectionToClient() = default;
			virtual ~AbstractConnectionToClient() = default;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			virtual void send(std::string message) =0;
			virtual void close() =0;
	};
}