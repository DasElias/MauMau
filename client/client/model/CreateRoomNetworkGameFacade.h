#pragma once
#include "NetworkGameFacade.h"

namespace card {
	class CreateRoomNetworkGameFacade : public NetworkGameFacade {
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CreateRoomNetworkGameFacade(NetworkErrorHandler& errorHandler, std::string username);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			void sendRequest(std::string username);
	};
}