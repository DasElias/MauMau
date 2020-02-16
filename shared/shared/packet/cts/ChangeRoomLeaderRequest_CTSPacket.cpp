#include "ChangeRoomLeaderRequest_CTSPacket.h"

namespace card {
	std::string const ChangeRoomLeaderRequest_CTSPacket::USERNAME_KEY = "43A";

	ChangeRoomLeaderRequest_CTSPacket::ChangeRoomLeaderRequest_CTSPacket(const std::string& usernameOfNewLeader) :
			ClientToServerPacket(PACKET_ID),
			username(usernameOfNewLeader) {
	}
	
	ChangeRoomLeaderRequest_CTSPacket::ChangeRoomLeaderRequest_CTSPacket(nlohmann::json& json) :
			ClientToServerPacket(json, PACKET_ID),
			username(json[USERNAME_KEY]) {

		throwErrorIfTypeIsIncorrect(json);
	}

	std::string ChangeRoomLeaderRequest_CTSPacket::getUsernameOfNewLeader() const {
		return username;
	}

	void ChangeRoomLeaderRequest_CTSPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		ClientToServerPacket::addJsonProperties(jsonHandle);
		jsonHandle[USERNAME_KEY] = username;
	}
}