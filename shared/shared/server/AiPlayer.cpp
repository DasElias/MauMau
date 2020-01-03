#include "AiPlayer.h"
#include "../utils/MathUtils.h"
#include "../utils/ThreadUtils.h"
#include <climits>

namespace card {
	AiPlayer::AiPlayer(std::shared_ptr<ParticipantOnServer> wrappedParticipant, ServerMauMauGame& game, std::vector<Card> handCards) :
			Player(wrappedParticipant, handCards),
			game(game) {
	}
	void AiPlayer::onStartTurn() {
		Player::onStartTurn();

		int delay = randomInRange(3000, 4000);
		threadUtils_invokeIn(delay, [this]() {
			performTurn();
		});
	}
	void AiPlayer::performTurn() {
		if(playCard()) return;

		// no card was played, so we have to draw one
		game.drawCardAndSetNextPlayerOnTurn(*this);
		return;
		throw std::runtime_error("Other player is already on turn! TODO FIX");

		// can we draw the played card?
		playCard();
	}
	
	bool AiPlayer::playCard() {
		for(auto& card : getHandCards()) {
			if(game.canPlay(*this, card)) {
				CardIndex nextCardIndex = (game.canChangeColor(card)) ? chooseCardIndex() : CardIndex::NULLINDEX;
				game.playCardAndSetNextPlayerOnTurn(*this, card, nextCardIndex);
				return true;
			}
		}
		return false;
	}
	CardIndex AiPlayer::chooseCardIndex() {
		std::map<CardIndex, uint8_t> cardIndexCounter;
		for(auto& card : getHandCards()) {
			if(cardIndexCounter.find(card.getCardIndex()) == cardIndexCounter.end()) {
				// CardIndex doesn't exist yet
				cardIndexCounter[card.getCardIndex()] = 1;
			} else {
				cardIndexCounter[card.getCardIndex()]++;
			}
		}

		uint8_t maxValue = 0;
		CardIndex maxCardIndex = CardIndex::NULLINDEX;

		for(auto& pair : cardIndexCounter) {
			if(pair.second > maxValue) {
				maxCardIndex = pair.first;
			}
		}

		return maxCardIndex;
	}
}