#include "KickPlayerRequest_CTSPacket.h"

namespace card {
	std::string const KickPlayerRequest_CTSPacket::USERNAME_KEY = "47A";

	KickPlayerRequest_CTSPacket::KickPlayerRequest_CTSPacket(const std::string& u) :
			ClientToServerPacket(PACKET_ID),
			username(u) {
	}
	
	KickPlayerRequest_CTSPacket::KickPlayerRequest_CTSPacket(nlohmann::json& json) :
			ClientToServerPacket(json, PACKET_ID),
			username(json[USERNAME_KEY]) {

		throwErrorIfTypeIsIncorrect(json);
	}

	std::string KickPlayerRequest_CTSPacket::getUsernameOfPlayerToKick() const {
		return username;
	}

	void KickPlayerRequest_CTSPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		ClientToServerPacket::addJsonProperties(jsonHandle);
		jsonHandle[USERNAME_KEY] = username;
	}
}