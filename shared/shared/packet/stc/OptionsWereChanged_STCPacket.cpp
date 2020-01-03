#include "OptionsWereChanged_STCPacket.h"

namespace card {
	std::string const OptionsWereChanged_STCPacket::OPTIONS_KEY = "options";

	OptionsWereChanged_STCPacket::OptionsWereChanged_STCPacket(std::map<std::string, int> options) :
			Packet(PACKET_ID),
			options(options) {
	}

	OptionsWereChanged_STCPacket::OptionsWereChanged_STCPacket(nlohmann::json jsonHandle) :
			Packet(PACKET_ID),
			options(jsonHandle.at(OPTIONS_KEY).get<std::map<std::string, int>>()) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}

	std::map<std::string, int> OptionsWereChanged_STCPacket::getOptions() const {
		return options;
	}

	void OptionsWereChanged_STCPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		Packet::addJsonProperties(jsonHandle);
		jsonHandle[OPTIONS_KEY] = this->options;
	}

}