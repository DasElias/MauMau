#pragma once
#include <service/AbstractConnectionToClient.h>
#include <string>

namespace card {
	class AbstractConnectionToClientDummy : public AbstractConnectionToClient {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			bool wasClosed_field = false;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void send(std::string message);
			void close();
			bool wasClosed() const;
	};
}