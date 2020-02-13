#include "LocalGameFacade.h"

namespace card {
	LocalGameFacade::LocalGameFacade(std::string username, std::size_t amountOfOpponents, Avatar avatar, RoomOptions options) :
			localParticipantOnServer(std::make_shared<ParticipantOnServer>(username, avatar)),
			packetTransmitter(std::make_shared<LocalPacketTransmitter>(localParticipantOnServer)),
			server(packetTransmitter, amountOfOpponents, localParticipantOnServer, options),
			room(packetTransmitter, server.getOpponentUsernames(), server.getOpponentAvatars(), username, avatar, username, options) {
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