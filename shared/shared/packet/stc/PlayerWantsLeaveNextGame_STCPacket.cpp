#include "PlayerWantsLeaveNextGame_STCPacket.h"

namespace card {
	PlayerWantsLeaveNextGame_STCPacket::PlayerWantsLeaveNextGame_STCPacket(std::string username) :
			AbstractUsernamePacket_STCPacket(username, PACKET_ID) {
	}

	PlayerWantsLeaveNextGame_STCPacket::PlayerWantsLeaveNextGame_STCPacket(nlohmann::json& jsonHandle) :
			AbstractUsernamePacket_STCPacket(jsonHandle, PACKET_ID) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}
}