#include "PlayCardRequest_CTSPacket.h"

namespace card {
	std::string const PlayCardRequest_CTSPacket::CARD_NUMBER_KEY = "cardNumber";
	std::string const PlayCardRequest_CTSPacket::NEW_CARD_INDEX_KEY = "newCardIndex";


	PlayCardRequest_CTSPacket::PlayCardRequest_CTSPacket(int cardNumber, int newCardIndex) :
			ClientToServerPacket(PACKET_ID),
			cardNumber(cardNumber),
			newCardIndex(newCardIndex) {
	}
	PlayCardRequest_CTSPacket::PlayCardRequest_CTSPacket(nlohmann::json jsonHandle) :
			ClientToServerPacket(jsonHandle, PACKET_ID),
			cardNumber(jsonHandle[CARD_NUMBER_KEY]),
			newCardIndex(jsonHandle[NEW_CARD_INDEX_KEY]) {
	}
	int PlayCardRequest_CTSPacket::getCardNumber() const {
		return cardNumber;
	}
	int PlayCardRequest_CTSPacket::getNewCardIndex() const {
		return newCardIndex;
	}
	void PlayCardRequest_CTSPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		ClientToServerPacket::addJsonProperties(jsonHandle);
		jsonHandle[CARD_NUMBER_KEY] = cardNumber;
		jsonHandle[NEW_CARD_INDEX_KEY] = newCardIndex;
	}
}