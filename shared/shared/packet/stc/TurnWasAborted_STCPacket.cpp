#include "TurnWasAborted_STCPacket.h"

namespace card {
	std::string const TurnWasAborted_STCPacket::CARDS_TO_DRAW_KEY = "24A";
	std::string const TurnWasAborted_STCPacket::AMOUNT_OF_CARDS_TO_DRAW_BEFORE_KEY = "24B";

	TurnWasAborted_STCPacket::TurnWasAborted_STCPacket(std::vector<int>& cardsToDraw, int amountOfCardsToDrawBefore) :
			Packet(PACKET_ID),
			cardsToDraw(cardsToDraw),
			amountOfCardsToDrawBefore(amountOfCardsToDrawBefore) {
	}
	TurnWasAborted_STCPacket::TurnWasAborted_STCPacket(nlohmann::json& jsonHandle) :
			Packet(PACKET_ID),
			cardsToDraw(jsonHandle.at(CARDS_TO_DRAW_KEY).get<std::vector<int>>()),
			amountOfCardsToDrawBefore(jsonHandle[AMOUNT_OF_CARDS_TO_DRAW_BEFORE_KEY]) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}
	std::vector<int> TurnWasAborted_STCPacket::getCardsToDraw() const {
		return cardsToDraw;
	}
	int TurnWasAborted_STCPacket::getAmountOfCardsToDrawBefore() const {
		return amountOfCardsToDrawBefore;
	}
	void TurnWasAborted_STCPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		Packet::addJsonProperties(jsonHandle);
		jsonHandle[CARDS_TO_DRAW_KEY] = cardsToDraw;
		jsonHandle[AMOUNT_OF_CARDS_TO_DRAW_BEFORE_KEY] = amountOfCardsToDrawBefore;
	} 
}