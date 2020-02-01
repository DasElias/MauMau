#include "TurnWasAborted_STCPacket.h"

namespace card {
	std::string const TurnWasAborted_STCPacket::CARDS_TO_DRAW_KEY = "24A";

	TurnWasAborted_STCPacket::TurnWasAborted_STCPacket(std::vector<int>& cardsToDraw) :
			Packet(PACKET_ID),
			cardsToDraw(cardsToDraw) {
	}
	TurnWasAborted_STCPacket::TurnWasAborted_STCPacket(nlohmann::json& jsonHandle) :
			Packet(PACKET_ID),
			cardsToDraw(jsonHandle.at(CARDS_TO_DRAW_KEY).get<std::vector<int>>()) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}
	std::vector<int> TurnWasAborted_STCPacket::getCardsToDraw() const {
		return cardsToDraw;
	}
	void TurnWasAborted_STCPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		Packet::addJsonProperties(jsonHandle);
		jsonHandle[CARDS_TO_DRAW_KEY] = cardsToDraw;
	} 
}