#include "OtherPlayerHasPlayedCard_STCPacket.h"

namespace card {
	std::string const OtherPlayerHasPlayedCard_STCPacket::CARD_NUMBER_KEY = "cardNumber";
	std::string const OtherPlayerHasPlayedCard_STCPacket::NEW_CARD_INDEX_KEY = "newCardIndex";
	std::string const OtherPlayerHasPlayedCard_STCPacket::CARDS_TO_DRAW_KEY = "cardsToDraw";
	std::string const OtherPlayerHasPlayedCard_STCPacket::WAS_DRAWN_KEY = "wasDrawnBeforePlayed";
	
	OtherPlayerHasPlayedCard_STCPacket::OtherPlayerHasPlayedCard_STCPacket(std::string username, int cardNumber, int newCardIndex, std::vector<int> cardsToDraw, bool wasDrawnBeforePlayed) :
			AbstractUsernamePacket_STCPacket(username, PACKET_ID),
			cardNumber(cardNumber),
			newCardIndex(newCardIndex),
			cardsToDraw(cardsToDraw),
			wasDrawnBeforePlayed_field(wasDrawnBeforePlayed) {
	}
	OtherPlayerHasPlayedCard_STCPacket::OtherPlayerHasPlayedCard_STCPacket(nlohmann::json jsonHandle) :
			AbstractUsernamePacket_STCPacket(jsonHandle, PACKET_ID),
			cardNumber(jsonHandle[CARD_NUMBER_KEY]),
			newCardIndex(jsonHandle[NEW_CARD_INDEX_KEY]),
			cardsToDraw(jsonHandle.at(CARDS_TO_DRAW_KEY).get<std::vector<int>>()),
			wasDrawnBeforePlayed_field(jsonHandle[WAS_DRAWN_KEY]) {

		throwErrorIfTypeIsIncorrect(jsonHandle);
	}
	int OtherPlayerHasPlayedCard_STCPacket::getCardNumber() const {
		return cardNumber;
	}
	int OtherPlayerHasPlayedCard_STCPacket::getNewCardIndex() const {
		return newCardIndex;
	}
	std::vector<int> OtherPlayerHasPlayedCard_STCPacket::getCardsToDraw() const {
		return cardsToDraw;
	}
	bool OtherPlayerHasPlayedCard_STCPacket::wasDrawnBeforePlayed() const {
		return wasDrawnBeforePlayed_field;
	}
	void OtherPlayerHasPlayedCard_STCPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		AbstractUsernamePacket_STCPacket::addJsonProperties(jsonHandle);
		jsonHandle[CARD_NUMBER_KEY] = cardNumber;
		jsonHandle[NEW_CARD_INDEX_KEY] = newCardIndex;
		jsonHandle[CARDS_TO_DRAW_KEY] = cardsToDraw;
		jsonHandle[WAS_DRAWN_KEY] = wasDrawnBeforePlayed_field;
	}
}