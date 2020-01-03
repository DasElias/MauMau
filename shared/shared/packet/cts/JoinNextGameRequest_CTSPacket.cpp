#include "JoinNextGameRequest_CTSPacket.h"

namespace card {
	JoinNextGameRequest_CTSPacket::JoinNextGameRequest_CTSPacket() :
			ClientToServerPacket(PACKET_ID) {
	}

	JoinNextGameRequest_CTSPacket::JoinNextGameRequest_CTSPacket(nlohmann::json jsonHandle) :
			ClientToServerPacket(jsonHandle, PACKET_ID) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}
}