#include "OtherPlayerHasPassed_STCPacket.h"

namespace card {	
	OtherPlayerHasPassed_STCPacket::OtherPlayerHasPassed_STCPacket(std::string username) :
			AbstractUsernamePacket_STCPacket(username, PACKET_ID) {
	
	}
	OtherPlayerHasPassed_STCPacket::OtherPlayerHasPassed_STCPacket(nlohmann::json& jsonHandle) :
			AbstractUsernamePacket_STCPacket(jsonHandle, PACKET_ID) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}

}