#include "RoomCreationRequest_CTSPacket.h"

namespace card {
	std::string const RoomCreationRequest_CTSPacket::USERNAME_KEY = "username";

	RoomCreationRequest_CTSPacket::RoomCreationRequest_CTSPacket(std::string ownUsername) :
			ClientToServerPacket(PACKET_ID),
			ownUsername(ownUsername) {
	}
	RoomCreationRequest_CTSPacket::RoomCreationRequest_CTSPacket(nlohmann::json jsonHandle) :
			ClientToServerPacket(jsonHandle, PACKET_ID),
			ownUsername(jsonHandle[USERNAME_KEY]) {
	}
	std::string RoomCreationRequest_CTSPacket::getOwnUsername() const {
		return ownUsername;
	}
	void RoomCreationRequest_CTSPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		ClientToServerPacket::addJsonProperties(jsonHandle);
		jsonHandle[USERNAME_KEY] = ownUsername;
	}
}