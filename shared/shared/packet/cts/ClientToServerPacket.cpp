#include "ClientToServerPacket.h"
namespace card {

	ClientToServerPacket::ClientToServerPacket(int packetType) :
			Packet(packetType) {
	}
	ClientToServerPacket::ClientToServerPacket(nlohmann::json& jsonHandle, int packetType) :
			Packet(packetType) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}
}