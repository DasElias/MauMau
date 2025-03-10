#include "LocalGameFacade.h"

namespace card {
	LocalGameFacade::LocalGameFacade(std::string username, std::size_t amountOfOpponents, Avatar avatar, RoomOptions options, std::unique_ptr<AbstractRoomLeaveHandler> roomLeaveHandler, std::unique_ptr<AbstractReturnBackToMenuHandler> gameEndHandler) :
			localParticipantOnServer(std::make_shared<ParticipantOnServer>(username, avatar)),
			packetTransmitter(std::make_shared<LocalPacketTransmitter>(localParticipantOnServer)),
			roomLeaveHandler(std::move(roomLeaveHandler)),
			gameEndHandler(std::move(gameEndHandler)),
			server(packetTransmitter, amountOfOpponents, localParticipantOnServer, options),
			room(packetTransmitter, *(this->roomLeaveHandler), *(this->gameEndHandler), 0, server.getOpponentUsernames(), server.getOpponentAvatars(), std::vector<bool>(amountOfOpponents, true), username, avatar, username, options) {
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