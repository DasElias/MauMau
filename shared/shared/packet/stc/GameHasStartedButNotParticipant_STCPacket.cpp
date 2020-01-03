#include "GameHasStartedButNotParticipant_STCPacket.h"

namespace card {
	GameHasStartedButNotParticipant_STCPacket::GameHasStartedButNotParticipant_STCPacket() :
			Packet(PACKET_ID) {
	}

	GameHasStartedButNotParticipant_STCPacket::GameHasStartedButNotParticipant_STCPacket(nlohmann::json jsonHandle) :
			Packet(PACKET_ID) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}
}