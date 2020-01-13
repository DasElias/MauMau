#include "ChangeOptionsRequest_CTSPacket.h"

namespace card {
	std::string const ChangeOptionsRequest_CTSPacket::OPTIONS_KEY = "42A";

	ChangeOptionsRequest_CTSPacket::ChangeOptionsRequest_CTSPacket(std::map<std::string, int>& options) :
			ClientToServerPacket(PACKET_ID),
			options(options) {
	}

	ChangeOptionsRequest_CTSPacket::ChangeOptionsRequest_CTSPacket(nlohmann::json& jsonHandle) :
			ClientToServerPacket(jsonHandle, PACKET_ID),
			options(jsonHandle.at(OPTIONS_KEY).get<std::map<std::string, int>>()) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}

	std::map<std::string, int> ChangeOptionsRequest_CTSPacket::getOptions() const {
		return options;
	}

	void ChangeOptionsRequest_CTSPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		ClientToServerPacket::addJsonProperties(jsonHandle);
		jsonHandle[OPTIONS_KEY] = this->options;
	}


}