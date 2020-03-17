#include "GameWasAborted_STCPacket.h"

namespace card {	
	GameWasAborted_STCPacket::GameWasAborted_STCPacket() :
			Packet(PACKET_ID) {
	
	}
	GameWasAborted_STCPacket::GameWasAborted_STCPacket(nlohmann::json& jsonHandle) :
			Packet(PACKET_ID) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}

}