#include "GameHasBeenStarted_STCPacket.h"

namespace card {
	std::string const GameHasBeenStarted_STCPacket::ALL_PARTICIPANTS_KEY = "allParticipants";
	std::string const GameHasBeenStarted_STCPacket::USERNAME_ON_TURN_KEY = "usernameOnTurn";
	std::string const GameHasBeenStarted_STCPacket::HAND_CARDS_KEY = "handCards";
	std::string const GameHasBeenStarted_STCPacket::START_CARD_KEY = "startCard";
	std::string const GameHasBeenStarted_STCPacket::NEXT_CARD_ON_DRAW_STCK = "nextCardOnDrawStack";


	GameHasBeenStarted_STCPacket::GameHasBeenStarted_STCPacket(std::vector<std::string> allParticipants, std::string usernameOnTurn, std::vector<int> handCards, int startCard, int nextCardOnDrawStack) :
			Packet(PACKET_ID),
			allParticipants(allParticipants),
			usernameOnTurn(usernameOnTurn),
			handCards(handCards),
			startCard(startCard),
			nextCardOnDrawStack(nextCardOnDrawStack) {
	}
	GameHasBeenStarted_STCPacket::GameHasBeenStarted_STCPacket(nlohmann::json json) :
			GameHasBeenStarted_STCPacket(json[ALL_PARTICIPANTS_KEY], json[USERNAME_ON_TURN_KEY], json[HAND_CARDS_KEY], json[START_CARD_KEY], json[NEXT_CARD_ON_DRAW_STCK]) {

		throwErrorIfTypeIsIncorrect(json);
	}

	std::vector<std::string> GameHasBeenStarted_STCPacket::getUsernamesOfAllParticipants() const {
		return allParticipants;
	}
	std::string GameHasBeenStarted_STCPacket::getUsernameOnTurn() const {
		return usernameOnTurn;
	}
	std::vector<int> GameHasBeenStarted_STCPacket::getHandCards() const {
		return handCards;
	}
	int GameHasBeenStarted_STCPacket::getStartCard() const {
		return startCard;
	}
	int GameHasBeenStarted_STCPacket::getNextCardOnDrawStack() const {
		return nextCardOnDrawStack;
	}
	void GameHasBeenStarted_STCPacket::addJsonProperties(nlohmann::json& jsonHandle) const {
		Packet::addJsonProperties(jsonHandle);

		jsonHandle[ALL_PARTICIPANTS_KEY] = allParticipants;
		jsonHandle[USERNAME_ON_TURN_KEY] = usernameOnTurn;
		jsonHandle[HAND_CARDS_KEY] = handCards;
		jsonHandle[START_CARD_KEY] = startCard;
		jsonHandle[NEXT_CARD_ON_DRAW_STCK] = nextCardOnDrawStack;
	}

}