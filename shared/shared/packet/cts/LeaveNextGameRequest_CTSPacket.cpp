#include "LeaveNextGameRequest_CTSPacket.h"

namespace card {
	LeaveNextGameRequest_CTSPacket::LeaveNextGameRequest_CTSPacket() :
			ClientToServerPacket(PACKET_ID) {
	}

	LeaveNextGameRequest_CTSPacket::LeaveNextGameRequest_CTSPacket(nlohmann::json& jsonHandle) :
			ClientToServerPacket(jsonHandle, PACKET_ID) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}
}