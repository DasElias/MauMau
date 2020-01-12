#pragma once
#include "AbstractGameFacade.h"
#include "ProxyRoom.h"
#include "../network/LocalPacketTransmitter.h"
#include "../localServer/LocalServer.h"
#include "ParticipantOnClient.h"

namespace card {
	class LocalGameFacade : public AbstractGameFacade {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<ParticipantOnServer> localParticipantOnServer;
			std::shared_ptr<LocalPacketTransmitter> packetTransmitter;
			LocalServer server;
			ProxyRoom room;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			LocalGameFacade(std::string username, Avatar avatar);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			bool isGameRunning() const override;
			ProxyMauMauGame& getGame() override;
			ProxyRoom& getRoom() override;
	};
}