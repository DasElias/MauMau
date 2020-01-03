#pragma once
#include "NetworkGameFacade.h"
#include <shared/model/RoomCode.h>

namespace card {
	class JoinRoomNetworkGameFacade : public NetworkGameFacade {
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			JoinRoomNetworkGameFacade(NetworkErrorHandler& errorHandler, std::string username, RoomCode roomCode);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			void sendRequest(std::string username, RoomCode roomCode);
	};
}