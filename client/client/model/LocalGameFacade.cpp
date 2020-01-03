#include "LocalGameFacade.h"

namespace card {
	LocalGameFacade::LocalGameFacade(std::string username) :
			localParticipantOnServer(std::make_shared<ParticipantOnServer>(username)),
			packetTransmitter(std::make_shared<LocalPacketTransmitter>(localParticipantOnServer)),
			server(packetTransmitter, localParticipantOnServer),
			room(packetTransmitter, server.getOpponentUsernames(), username, username, {}) {
	}

	bool LocalGameFacade::isGameRunning() const {
		return room.isGameRunning();
	}

	ProxyMauMauGame& LocalGameFacade::getGame() {
		return room.getGame();
	}

	ProxyRoom& LocalGameFacade::getRoom() {
		return room;
	}

}