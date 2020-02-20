#include "JoinAiPlayerRequest_CTSPacket.h"

namespace card {
	JoinAiPlayerRequest_CTSPacket::JoinAiPlayerRequest_CTSPacket() :
			ClientToServerPacket(PACKET_ID) {
	}

	JoinAiPlayerRequest_CTSPacket::JoinAiPlayerRequest_CTSPacket(nlohmann::json& jsonHandle) :
			ClientToServerPacket(jsonHandle, PACKET_ID) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}
}