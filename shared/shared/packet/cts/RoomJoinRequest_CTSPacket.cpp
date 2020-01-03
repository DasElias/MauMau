#include "RoomJoinRequest_CTSPacket.h"

namespace card {
	std::string const RoomJoinRequest_CTSPacket::ROOM_CODE_KEY = "roomCode";
	std::string const RoomJoinRequest_CTSPacket::USERNAME_KEY = "username";

	RoomJoinRequest_CTSPacket::RoomJoinRequest_CTSPacket(std::string ownUsername, RoomCode roomCode) :
			ClientToServerPacket(PACKET_ID),
			ownUsername(ownUsername),
			roomCode(roomCode) {
	}
	RoomJoinRequest_CTSPacket::RoomJoinRequest_CTSPacket(nlohmann::json jsonHandle) :
			ClientToServerPacket(jsonHandle, PACKET_ID),
			ownUsername(jsonHandle[USERNAME_KEY]),
			roomCode(jsonHandle[ROOM_CODE_KEY]) {
	}
	std::string RoomJoinRequest_CTSPacket::getOwnUsername() const {
		return ownUsername;
	}
	RoomCode RoomJoinRequest_CTSPacket::getRoomCode() const {
		return roomCode;
	}
	void RoomJoinRequest_CTSPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		ClientToServerPacket::addJsonProperties(jsonHandle);
		jsonHandle[USERNAME_KEY] = ownUsername;
		jsonHandle[ROOM_CODE_KEY] = roomCode;
	}
}