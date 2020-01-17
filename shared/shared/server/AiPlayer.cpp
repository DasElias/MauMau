#include "AiPlayer.h"
#include "../utils/MathUtils.h"
#include "../utils/ThreadUtils.h"
#include <climits>
#include "../model/MauMauCardValueMeanings.h"
#include "../model/CardAnimationDuration.h"

namespace card {
	AiPlayer::AiPlayer(std::shared_ptr<ParticipantOnServer> wrappedParticipant, ServerMauMauGame& game, std::vector<Card> handCards) :
			Player(wrappedParticipant, handCards),
			game(game) {
	}
	void AiPlayer::onStartTurn() {
		Player::onStartTurn();

		int delay = randomInRange(2000, 3000);
		if(game.getPlayCardStack().getSize() > 1 && game.getPlayCardStack().getLast().getValue() == DRAW_2_VALUE) {
			delay += getDelayUntilTwoCardsAreDrawed();
		}
		if(game.wasCardDrawnAndPlayedLastTurn()) {
			delay += DRAW_DURATION_MS + DELAY_BETWEEN_DRAW_AND_PLAY;
		}
		threadUtils_invokeIn(delay, [this]() {
			performTurn();
		});
	}
	void AiPlayer::performTurn() {
		if(tryPlayCard()) return;

		// player cannot play, therefore no card was played
		if(shouldPlayDrawnCard()) {
			Card drawnCard = game.getDrawCardStack().getLast();
			playCardImpl(drawnCard);
		} else {
			game.drawCardAndSetNextPlayerOnTurn(*this);
		}
	}
	
	bool AiPlayer::tryPlayCard() {
		for(auto& card : getHandCards()) {
			if(game.canPlay(*this, card)) {
				playCardImpl(card);
				return true;
			}
		}
		return false;
	}
	void AiPlayer::playCardImpl(Card card) {
		CardIndex nextCardIndex = (game.canChangeColor(card)) ? chooseCardIndex() : CardIndex::NULLINDEX;
		game.playCardAndSetNextPlayerOnTurn(*this, card, nextCardIndex);
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
	bool AiPlayer::shouldPlayDrawnCard() {
		Card lastCardOnDrawStack = game.getDrawCardStack().getLast();
		return game.canPlay(*this, lastCardOnDrawStack);
	}
}