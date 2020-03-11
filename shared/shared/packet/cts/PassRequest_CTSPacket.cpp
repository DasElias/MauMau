#include "PassRequest_CTSPacket.h"

namespace card {
	PassRequest_CTSPacket::PassRequest_CTSPacket() :
			ClientToServerPacket(PACKET_ID) {
	}

	PassRequest_CTSPacket::PassRequest_CTSPacket(nlohmann::json& jsonHandle) :
			ClientToServerPacket(jsonHandle, PACKET_ID) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}
}