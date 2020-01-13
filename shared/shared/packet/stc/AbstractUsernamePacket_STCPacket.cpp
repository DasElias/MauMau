#include "AbstractUsernamePacket_STCPacket.h"

namespace card {
	std::string const AbstractUsernamePacket_STCPacket::USERNAME_KEY = "UNM";

	AbstractUsernamePacket_STCPacket::AbstractUsernamePacket_STCPacket(std::string username, int packetType) :
			Packet(packetType),
			username(username) {
	}

	AbstractUsernamePacket_STCPacket::AbstractUsernamePacket_STCPacket(nlohmann::json& json, int packetType) :
			Packet(packetType),
			username(json[USERNAME_KEY]) {

		throwErrorIfTypeIsIncorrect(json);
	}

	std::string AbstractUsernamePacket_STCPacket::getUsername() const {
		return username;
	}

	void AbstractUsernamePacket_STCPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		Packet::addJsonProperties(jsonHandle);
		jsonHandle[USERNAME_KEY] = username;
	}
}