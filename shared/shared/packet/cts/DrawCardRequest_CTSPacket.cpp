#include "DrawCardRequest_CTSPacket.h"

namespace card {
	DrawCardRequest_CTSPacket::DrawCardRequest_CTSPacket() :
			ClientToServerPacket(PACKET_ID) {
	}
	DrawCardRequest_CTSPacket::DrawCardRequest_CTSPacket(nlohmann::json jsonHandle) :
			ClientToServerPacket(jsonHandle, PACKET_ID) {
	}
}