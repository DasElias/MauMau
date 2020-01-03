#include "RoomLeaderHasChanged_STCPacket.h"

namespace card {
	RoomLeaderHasChanged_STCPacket::RoomLeaderHasChanged_STCPacket(std::string username) :
			AbstractUsernamePacket_STCPacket(username, PACKET_ID) {
	}

	RoomLeaderHasChanged_STCPacket::RoomLeaderHasChanged_STCPacket(nlohmann::json jsonHandle) :
			AbstractUsernamePacket_STCPacket(jsonHandle, PACKET_ID) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}
}