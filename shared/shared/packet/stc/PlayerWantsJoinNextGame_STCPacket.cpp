#include "PlayerWantsJoinNextGame_STCPacket.h"

namespace card {
	PlayerWantsJoinNextGame_STCPacket::PlayerWantsJoinNextGame_STCPacket(std::string username) :
			AbstractUsernamePacket_STCPacket(username, PACKET_ID) {
	}

	PlayerWantsJoinNextGame_STCPacket::PlayerWantsJoinNextGame_STCPacket(nlohmann::json jsonHandle) :
			AbstractUsernamePacket_STCPacket(jsonHandle, PACKET_ID) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}
}