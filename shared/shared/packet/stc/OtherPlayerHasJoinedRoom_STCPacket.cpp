#include "OtherPlayerHasJoinedRoom_STCPacket.h"

namespace card {
	std::string const OtherPlayerHasJoinedRoom_STCPacket::AVATAR_KEY = "avatar";

	OtherPlayerHasJoinedRoom_STCPacket::OtherPlayerHasJoinedRoom_STCPacket(std::string username, Avatar avatar) :
			AbstractUsernamePacket_STCPacket(username, PACKET_ID),
			avatar(avatar) {
	}

	OtherPlayerHasJoinedRoom_STCPacket::OtherPlayerHasJoinedRoom_STCPacket(nlohmann::json jsonHandle) :
			AbstractUsernamePacket_STCPacket(jsonHandle, PACKET_ID),
			avatar(jsonHandle[AVATAR_KEY]) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}
	Avatar OtherPlayerHasJoinedRoom_STCPacket::getAvatar() const {
		return avatar;
	}
	void OtherPlayerHasJoinedRoom_STCPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		AbstractUsernamePacket_STCPacket::addJsonProperties(jsonHandle);

		jsonHandle[AVATAR_KEY] = avatar;
	}
}