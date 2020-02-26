#include "GameHasBeenStarted_STCPacket.h"

namespace card {
	std::string const GameHasBeenStarted_STCPacket::ALL_PARTICIPANTS_KEY = "15A";
	std::string const GameHasBeenStarted_STCPacket::HAND_CARDS_KEY = "15C";
	std::string const GameHasBeenStarted_STCPacket::START_CARD_KEY = "15D";


	GameHasBeenStarted_STCPacket::GameHasBeenStarted_STCPacket(std::vector<std::string> allParticipants, std::vector<int> handCards, int startCard) :
			Packet(PACKET_ID),
			allParticipants(allParticipants),
			handCards(handCards),
			startCard(startCard) {
	}
	GameHasBeenStarted_STCPacket::GameHasBeenStarted_STCPacket(nlohmann::json& json) :
			GameHasBeenStarted_STCPacket(json[ALL_PARTICIPANTS_KEY], json[HAND_CARDS_KEY], json[START_CARD_KEY]) {

		throwErrorIfTypeIsIncorrect(json);
	}

	std::vector<std::string> GameHasBeenStarted_STCPacket::getUsernamesOfAllParticipants() const {
		return allParticipants;
	}
	std::vector<int> GameHasBeenStarted_STCPacket::getHandCards() const {
		return handCards;
	}
	int GameHasBeenStarted_STCPacket::getStartCard() const {
		return startCard;
	}
	void GameHasBeenStarted_STCPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		Packet::addJsonProperties(jsonHandle);

		jsonHandle[ALL_PARTICIPANTS_KEY] = allParticipants;
		jsonHandle[HAND_CARDS_KEY] = handCards;
		jsonHandle[START_CARD_KEY] = startCard;
	}

}