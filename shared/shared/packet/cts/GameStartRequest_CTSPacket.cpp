#include "GameStartRequest_CTSPacket.h"

namespace card {
	GameStartRequest_CTSPacket::GameStartRequest_CTSPacket() :
			ClientToServerPacket(PACKET_ID) {
	}

	GameStartRequest_CTSPacket::GameStartRequest_CTSPacket(nlohmann::json jsonHandle) :
			ClientToServerPacket(jsonHandle, PACKET_ID) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}
}