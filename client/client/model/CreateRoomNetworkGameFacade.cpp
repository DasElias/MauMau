#include "CreateRoomNetworkGameFacade.h"
#include <shared/packet/cts/RoomCreationRequest_CTSPacket.h>

namespace card {
	CreateRoomNetworkGameFacade::CreateRoomNetworkGameFacade(NetworkErrorHandler& errorHandler, std::string username, Avatar avatar) :	
			NetworkGameFacade(errorHandler, username, avatar) {

		if(! hasErrorOccuredOnEstablishingConnection()) sendRequest(username, avatar);
	}
	void CreateRoomNetworkGameFacade::sendRequest(std::string username, Avatar avatar) {
		RoomCreationRequest_CTSPacket packet(username, avatar);
		auto packetTransmitter = getPacketTransmitter();
		packetTransmitter->sendPacketToServer(packet);
	}
}