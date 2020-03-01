#include "JoinRoomNetworkGameFacade.h"
#include <shared/packet/cts/RoomJoinRequest_CTSPacket.h>

namespace card {
	JoinRoomNetworkGameFacade::JoinRoomNetworkGameFacade(NetworkErrorHandler& errorHandler, std::unique_ptr<AbstractRoomLeaveHandler> roomLeaveHandler, std::unique_ptr<AbstractReturnBackToMenuHandler> gameEndHandler, std::string username, Avatar avatar, RoomCode roomCode) :
			NetworkGameFacade(errorHandler, std::move(roomLeaveHandler), std::move(gameEndHandler), username, avatar) {

		if(! hasErrorOccuredOnEstablishingConnection()) sendRequest(username, avatar, roomCode);
	}
	void JoinRoomNetworkGameFacade::sendRequest(std::string username, Avatar avatar, RoomCode roomCode) {
		RoomJoinRequest_CTSPacket packet(username, avatar, roomCode);
		auto packetTransmitter = getPacketTransmitter();
		packetTransmitter->sendPacketToServer(packet);
	}
}