#include "OtherPlayerHasDrawnCards_STCPacket.h"

namespace card {	
	OtherPlayerHasDrawnCards_STCPacket::OtherPlayerHasDrawnCards_STCPacket(std::string username) :
			AbstractUsernamePacket_STCPacket(username, PACKET_ID) {
	
	}
	OtherPlayerHasDrawnCards_STCPacket::OtherPlayerHasDrawnCards_STCPacket(nlohmann::json& jsonHandle) :
			AbstractUsernamePacket_STCPacket(jsonHandle, PACKET_ID) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}

}