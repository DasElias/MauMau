#include "InitialPlayerIsOnTurn_STCPacket.h"

namespace card {	
	std::string const InitialPlayerIsOnTurn_STCPacket::NEXT_CARD_ON_DRAW_STCK = "27A";

	InitialPlayerIsOnTurn_STCPacket::InitialPlayerIsOnTurn_STCPacket(std::string username, int nextCardOnDrawStack) :
			AbstractUsernamePacket_STCPacket(username, PACKET_ID),
			nextCardOnDrawStack(nextCardOnDrawStack) {
	
	}
	InitialPlayerIsOnTurn_STCPacket::InitialPlayerIsOnTurn_STCPacket(nlohmann::json& jsonHandle) :
			AbstractUsernamePacket_STCPacket(jsonHandle, PACKET_ID),
			nextCardOnDrawStack(jsonHandle[NEXT_CARD_ON_DRAW_STCK]) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}
	int InitialPlayerIsOnTurn_STCPacket::getNextCardOnDrawStack() const {
		return nextCardOnDrawStack;
	}
	void InitialPlayerIsOnTurn_STCPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		AbstractUsernamePacket_STCPacket::addJsonProperties(jsonHandle);

		jsonHandle[NEXT_CARD_ON_DRAW_STCK] = nextCardOnDrawStack;
	}


}