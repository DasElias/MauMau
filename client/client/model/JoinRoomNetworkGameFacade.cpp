#include "JoinRoomNetworkGameFacade.h"
#include <shared/packet/cts/RoomJoinRequest_CTSPacket.h>

namespace card {
	JoinRoomNetworkGameFacade::JoinRoomNetworkGameFacade(NetworkErrorHandler& errorHandler, std::string username, Avatar avatar, RoomCode roomCode) :
			NetworkGameFacade(errorHandler, username, avatar) {

		if(! hasErrorOccuredOnEstablishingConnection()) sendRequest(username, avatar, roomCode);
	}
	void JoinRoomNetworkGameFacade::sendRequest(std::string username, Avatar avatar, RoomCode roomCode) {
		RoomJoinRequest_CTSPacket packet(username, avatar, roomCode);
		auto packetTransmitter = getPacketTransmitter();
		packetTransmitter->sendPacketToServer(packet);
	}
}