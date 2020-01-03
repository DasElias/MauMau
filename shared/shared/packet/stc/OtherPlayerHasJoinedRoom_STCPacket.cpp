#include "OtherPlayerHasJoinedRoom_STCPacket.h"

namespace card {
	OtherPlayerHasJoinedRoom_STCPacket::OtherPlayerHasJoinedRoom_STCPacket(std::string username) :
			AbstractUsernamePacket_STCPacket(username, PACKET_ID) {
	}

	OtherPlayerHasJoinedRoom_STCPacket::OtherPlayerHasJoinedRoom_STCPacket(nlohmann::json jsonHandle) :
			AbstractUsernamePacket_STCPacket(jsonHandle, PACKET_ID) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}
}