#include "CreateRoomNetworkGameFacade.h"
#include <shared/packet/cts/RoomCreationRequest_CTSPacket.h>

namespace card {
	CreateRoomNetworkGameFacade::CreateRoomNetworkGameFacade(NetworkErrorHandler& errorHandler, std::unique_ptr<AbstractRoomLeaveHandler> roomLeaveHandler, std::unique_ptr<AbstractReturnBackToMenuHandler> gameEndHandler, std::string username, Avatar avatar, RoomOptions roomOptions) :
			NetworkGameFacade(errorHandler, std::move(roomLeaveHandler), std::move(gameEndHandler), username, avatar) {

		if(! hasErrorOccuredOnEstablishingConnection()) sendRequest(username, avatar, roomOptions);
	}
	void CreateRoomNetworkGameFacade::sendRequest(std::string username, Avatar avatar, RoomOptions roomOptions) {
		RoomCreationRequest_CTSPacket packet(username, avatar, roomOptions.getAllOptions());
		auto packetTransmitter = getPacketTransmitter();
		packetTransmitter->sendPacketToServer(packet);
	}
}