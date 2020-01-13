#include "Packet.h"

namespace card {
	std::string const Packet::PACKET_TYPE_KEY = "pType";

	Packet::Packet(int packetType) :
			packetType(packetType) {
	}
	int Packet::getPacketType(nlohmann::json& jsonHandle) {
		return jsonHandle[PACKET_TYPE_KEY];
	}
	std::string Packet::getJson() const {
		nlohmann::json jsonHandle;
		addJsonProperties(jsonHandle);
		return jsonHandle.dump();
	}
	int Packet::getPacketType() const {
		return packetType;
	}
	void Packet::throwErrorIfTypeIsIncorrect(nlohmann::json& jsonHandle) const {
		if(getPacketType(jsonHandle) != this->packetType) {
			throw std::runtime_error("Packet type is not consistent!");
		}
	}
	void Packet::addJsonProperties(nlohmann::json& jsonHandle) const {
		jsonHandle[PACKET_TYPE_KEY] = getPacketType();
	}
}
