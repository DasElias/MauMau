#include "RoomCreationRequest_CTSPacket.h"

namespace card {
	std::string const RoomCreationRequest_CTSPacket::USERNAME_KEY = "username";
	std::string const RoomCreationRequest_CTSPacket::AVATAR_KEY = "avatar";

	RoomCreationRequest_CTSPacket::RoomCreationRequest_CTSPacket(std::string ownUsername, Avatar avatar) :
			ClientToServerPacket(PACKET_ID),
			ownUsername(ownUsername),
			avatar(avatar) {
	}
	RoomCreationRequest_CTSPacket::RoomCreationRequest_CTSPacket(nlohmann::json jsonHandle) :
			ClientToServerPacket(jsonHandle, PACKET_ID),
			ownUsername(jsonHandle[USERNAME_KEY]),
			avatar(jsonHandle[AVATAR_KEY]) {
	}
	std::string RoomCreationRequest_CTSPacket::getOwnUsername() const {
		return ownUsername;
	}
	Avatar RoomCreationRequest_CTSPacket::getAvatar() const {
		return avatar; 
	}
	void RoomCreationRequest_CTSPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		ClientToServerPacket::addJsonProperties(jsonHandle);
		jsonHandle[USERNAME_KEY] = ownUsername;
		jsonHandle[AVATAR_KEY] = avatar;
	}
}