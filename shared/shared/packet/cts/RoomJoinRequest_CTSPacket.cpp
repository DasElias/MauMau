#include "RoomJoinRequest_CTSPacket.h"

namespace card {
	std::string const RoomJoinRequest_CTSPacket::ROOM_CODE_KEY = "40A";
	std::string const RoomJoinRequest_CTSPacket::USERNAME_KEY = "40B";
	std::string const RoomJoinRequest_CTSPacket::AVATAR_KEY = "40C";
	std::string const RoomJoinRequest_CTSPacket::CLIENT_PROTOCOL_VERSION_KEY = "40D";


	RoomJoinRequest_CTSPacket::RoomJoinRequest_CTSPacket(std::string ownUsername, Avatar avatar, RoomCode roomCode, std::string clientProtocolVersion) :
			ClientToServerPacket(PACKET_ID),
			ownUsername(ownUsername),
			roomCode(roomCode),
			avatar(avatar),
			clientProtocolVersion(clientProtocolVersion) {
	}
	RoomJoinRequest_CTSPacket::RoomJoinRequest_CTSPacket(nlohmann::json& jsonHandle) :
			ClientToServerPacket(jsonHandle, PACKET_ID),
			ownUsername(jsonHandle[USERNAME_KEY]),
			roomCode(jsonHandle[ROOM_CODE_KEY]),
			avatar(jsonHandle[AVATAR_KEY]),
			clientProtocolVersion(jsonHandle[CLIENT_PROTOCOL_VERSION_KEY]) {
	}
	std::string RoomJoinRequest_CTSPacket::getOwnUsername() const {
		return ownUsername;
	}
	RoomCode RoomJoinRequest_CTSPacket::getRoomCode() const {
		return roomCode;
	}
	Avatar RoomJoinRequest_CTSPacket::getAvatar() const {
		return avatar;
	}
	std::string RoomJoinRequest_CTSPacket::getClientProtocolVersion() const {
		return clientProtocolVersion;
	}
	void RoomJoinRequest_CTSPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		ClientToServerPacket::addJsonProperties(jsonHandle);
		jsonHandle[USERNAME_KEY] = ownUsername;
		jsonHandle[ROOM_CODE_KEY] = roomCode;
		jsonHandle[AVATAR_KEY] = avatar;
		jsonHandle[CLIENT_PROTOCOL_VERSION_KEY] = clientProtocolVersion;
	}
}