#pragma once
#include "NetworkGameFacade.h"
#include <shared/model/RoomCode.h>

namespace card {
	class JoinRoomNetworkGameFacade : public NetworkGameFacade {
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			JoinRoomNetworkGameFacade(NetworkErrorHandler& errorHandler, std::unique_ptr<AbstractRoomLeaveHandler> roomLeaveHandler, std::unique_ptr<AbstractReturnBackToMenuHandler> gameEndHandler, std::string username, Avatar avatar, RoomCode roomCode);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			void sendRequest(std::string username, Avatar avatar, RoomCode roomCode);
	};
}