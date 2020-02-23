#pragma once
#include "AbstractGameFacade.h"
#include "ProxyRoom.h"
#include "../network/LocalPacketTransmitter.h"
#include "LocalServer.h"
#include "ParticipantOnClient.h"

namespace card {
	class LocalGameFacade : public AbstractGameFacade {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<ParticipantOnServer> localParticipantOnServer;
			std::shared_ptr<LocalPacketTransmitter> packetTransmitter;
			std::unique_ptr<AbstractRoomLeaveHandler> gameEndHandler;
			LocalServer server;
			ProxyRoom room;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			LocalGameFacade(std::string username, std::size_t amountOfOpponents, Avatar avatar, RoomOptions options, std::unique_ptr<AbstractRoomLeaveHandler> gameEndHandler);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			bool isGameRunning() const override;
			ProxyMauMauGame& getGame() override;
			ProxyRoom& getRoom() override;
	};
}