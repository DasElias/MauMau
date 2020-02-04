#include "PlayCardRequest_CTSPacket.h"

namespace card {
	std::string const PlayCardRequest_CTSPacket::CARD_NUMBER_KEY = "50A";
	std::string const PlayCardRequest_CTSPacket::NEW_CARD_INDEX_KEY = "50B";
	std::string const PlayCardRequest_CTSPacket::WAS_JUST_DRAWN_KEY = "50C";

	PlayCardRequest_CTSPacket::PlayCardRequest_CTSPacket(int cardNumber, bool wasJustDrawn, int newCardIndex) :
			ClientToServerPacket(PACKET_ID),
			cardNumber(cardNumber),
			wasJustDrawn_field(wasJustDrawn),
			newCardIndex(newCardIndex) {
	}
	PlayCardRequest_CTSPacket::PlayCardRequest_CTSPacket(nlohmann::json& jsonHandle) :
			ClientToServerPacket(jsonHandle, PACKET_ID),
			cardNumber(jsonHandle[CARD_NUMBER_KEY]),
			wasJustDrawn_field(jsonHandle[WAS_JUST_DRAWN_KEY]),
			newCardIndex(jsonHandle[NEW_CARD_INDEX_KEY]) {
	}
	int PlayCardRequest_CTSPacket::getCardNumber() const {
		return cardNumber;
	}
	bool PlayCardRequest_CTSPacket::wasJustDrawn() const {
		return wasJustDrawn_field;
	}
	int PlayCardRequest_CTSPacket::getNewCardIndex() const {
		return newCardIndex;
	}
	void PlayCardRequest_CTSPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		ClientToServerPacket::addJsonProperties(jsonHandle);
		jsonHandle[CARD_NUMBER_KEY] = cardNumber;
		jsonHandle[WAS_JUST_DRAWN_KEY] = wasJustDrawn_field;
		jsonHandle[NEW_CARD_INDEX_KEY] = newCardIndex;
	}
}