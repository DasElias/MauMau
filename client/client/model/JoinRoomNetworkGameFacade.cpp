#include "JoinRoomNetworkGameFacade.h"
#include <shared/packet/cts/RoomJoinRequest_CTSPacket.h>

namespace card {
	JoinRoomNetworkGameFacade::JoinRoomNetworkGameFacade(NetworkErrorHandler& errorHandler, std::string username, RoomCode roomCode) :
			NetworkGameFacade(errorHandler, username) {

		if(! hasErrorOccuredOnEstablishingConnection()) sendRequest(username, roomCode);
	}
	void JoinRoomNetworkGameFacade::sendRequest(std::string username, RoomCode roomCode) {
		RoomJoinRequest_CTSPacket packet(username, roomCode);
		auto packetTransmitter = getPacketTransmitter();
		packetTransmitter->sendPacketToServer(packet);
	}
}