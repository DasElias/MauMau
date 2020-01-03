#include "LocalPlayerIsOnTurn_STCPacket.h"

namespace card {
	std::string const LocalPlayerIsOnTurn_STCPacket::NEXT_CARD_ON_DRAW_STCK = "nextCardOnDrawStack";
	
	LocalPlayerIsOnTurn_STCPacket::LocalPlayerIsOnTurn_STCPacket(int nextCardOnDrawStack) :
			Packet(PACKET_ID),
			nextCardOnDrawStack(nextCardOnDrawStack) {
	
	}
	LocalPlayerIsOnTurn_STCPacket::LocalPlayerIsOnTurn_STCPacket(nlohmann::json jsonHandle) :
			Packet(PACKET_ID),
			nextCardOnDrawStack(jsonHandle[NEXT_CARD_ON_DRAW_STCK]) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}
	int LocalPlayerIsOnTurn_STCPacket::getNextCardOnDrawStack() const {
		return nextCardOnDrawStack;
	}
	void LocalPlayerIsOnTurn_STCPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		Packet::addJsonProperties(jsonHandle);
		jsonHandle[NEXT_CARD_ON_DRAW_STCK] = nextCardOnDrawStack;
	}
}