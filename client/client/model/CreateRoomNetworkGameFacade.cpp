#include "CreateRoomNetworkGameFacade.h"
#include <shared/packet/cts/RoomCreationRequest_CTSPacket.h>

namespace card {
	CreateRoomNetworkGameFacade::CreateRoomNetworkGameFacade(NetworkErrorHandler& errorHandler, std::string username) :	
			NetworkGameFacade(errorHandler, username) {

		if(! hasErrorOccuredOnEstablishingConnection()) sendRequest(username);
	}
	void CreateRoomNetworkGameFacade::sendRequest(std::string username) {
		RoomCreationRequest_CTSPacket packet(username);
		auto packetTransmitter = getPacketTransmitter();
		packetTransmitter->sendPacketToServer(packet);
	}
}