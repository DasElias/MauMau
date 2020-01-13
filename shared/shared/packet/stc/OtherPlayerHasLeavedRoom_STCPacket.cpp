#include "OtherPlayerHasLeavedRoom_STCPacket.h"

namespace card {
	OtherPlayerHasLeavedRoom_STCPacket::OtherPlayerHasLeavedRoom_STCPacket(std::string username) :
			AbstractUsernamePacket_STCPacket(username, PACKET_ID) {
	}

	OtherPlayerHasLeavedRoom_STCPacket::OtherPlayerHasLeavedRoom_STCPacket(nlohmann::json& jsonHandle) :
			AbstractUsernamePacket_STCPacket(jsonHandle, PACKET_ID) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}
}