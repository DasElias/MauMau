#include "OtherPlayerHasJoinedRoom_STCPacket.h"

namespace card {
	std::string const OtherPlayerHasJoinedRoom_STCPacket::AVATAR_KEY = "10A";
	std::string const OtherPlayerHasJoinedRoom_STCPacket::IS_AI_PLAYER_KEY = "10B";

	OtherPlayerHasJoinedRoom_STCPacket::OtherPlayerHasJoinedRoom_STCPacket(std::string username, Avatar avatar, bool isAiPlayer) :
			AbstractUsernamePacket_STCPacket(username, PACKET_ID),
			avatar(avatar),
			field_isAiPlayer(isAiPlayer) {
	}

	OtherPlayerHasJoinedRoom_STCPacket::OtherPlayerHasJoinedRoom_STCPacket(nlohmann::json& jsonHandle) :
			AbstractUsernamePacket_STCPacket(jsonHandle, PACKET_ID),
			avatar(jsonHandle[AVATAR_KEY]),
			field_isAiPlayer(jsonHandle[IS_AI_PLAYER_KEY]) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}
	Avatar OtherPlayerHasJoinedRoom_STCPacket::getAvatar() const {
		return avatar;
	}
	bool OtherPlayerHasJoinedRoom_STCPacket::isAiPlayer() const {
		return field_isAiPlayer;
	}
	void OtherPlayerHasJoinedRoom_STCPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		AbstractUsernamePacket_STCPacket::addJsonProperties(jsonHandle);

		jsonHandle[AVATAR_KEY] = avatar;
		jsonHandle[IS_AI_PLAYER_KEY] = field_isAiPlayer;
	}
}