#include "MauRequest_CTSPacket.h"

namespace card {
	MauRequest_CTSPacket::MauRequest_CTSPacket() :
			ClientToServerPacket(PACKET_ID) {
	}

	MauRequest_CTSPacket::MauRequest_CTSPacket(nlohmann::json jsonHandle) :
			ClientToServerPacket(jsonHandle, PACKET_ID) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}
}