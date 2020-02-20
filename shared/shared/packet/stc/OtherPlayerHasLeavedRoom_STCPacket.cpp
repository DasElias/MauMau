#include "OtherPlayerHasLeavedRoom_STCPacket.h"

namespace card {
	std::string const OtherPlayerHasLeavedRoom_STCPacket::WAS_KICKED_KEY = "11A";

	OtherPlayerHasLeavedRoom_STCPacket::OtherPlayerHasLeavedRoom_STCPacket(std::string username, bool wasKicked) :
			AbstractUsernamePacket_STCPacket(username, PACKET_ID),
			wasKicked_field(wasKicked) {
	}

	OtherPlayerHasLeavedRoom_STCPacket::OtherPlayerHasLeavedRoom_STCPacket(nlohmann::json& jsonHandle) :
			AbstractUsernamePacket_STCPacket(jsonHandle, PACKET_ID),
			wasKicked_field(jsonHandle[WAS_KICKED_KEY]) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}
	bool OtherPlayerHasLeavedRoom_STCPacket::wasKicked() const {
		return wasKicked_field;
	}
	void OtherPlayerHasLeavedRoom_STCPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		AbstractUsernamePacket_STCPacket::addJsonProperties(jsonHandle);
		jsonHandle[WAS_KICKED_KEY] = wasKicked_field;
	}
}