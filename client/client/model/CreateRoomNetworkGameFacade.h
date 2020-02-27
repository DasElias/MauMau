#pragma once
#include "NetworkGameFacade.h"

namespace card {
	class CreateRoomNetworkGameFacade : public NetworkGameFacade {
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			CreateRoomNetworkGameFacade(NetworkErrorHandler& errorHandler, std::unique_ptr<AbstractRoomLeaveHandler> roomLeaveHandler, std::unique_ptr<AbstractClientGameEndHandler> gameEndHandler, std::string username, Avatar avatar, RoomOptions roomOptions);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		private:
			void sendRequest(std::string username, Avatar avatar, RoomOptions roomOptions);
	};
}