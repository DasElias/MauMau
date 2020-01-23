#include "AiPlayer.h"
#include "../utils/MathUtils.h"
#include "../utils/ThreadUtils.h"
#include <climits>
#include "../model/MauMauCardValueMeanings.h"
#include "../model/CardAnimationDuration.h"
#include "../model/TimeToSetNextPlayerOnTurnDuration.h"

namespace card {
	AiPlayer::AiPlayer(std::shared_ptr<ParticipantOnServer> wrappedParticipant, ServerMauMauGame& game, std::vector<Card> handCards) :
			Player(wrappedParticipant, handCards),
			game(game) {
	}
	void AiPlayer::onStartTurn() {
		Player::onStartTurn();

		int delay = randomInRange(1250, 1750);
		auto& playCardStack = game.getPlayCardStack();
		delay += getTimeToSetNextPlayerOnTurn(playCardStack.getSize(), playCardStack.getLast(), game.wasCardPlayedLastTurn(), game.wasCardDrawnLastTurn());
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
		std::vector<Card> playableCards = getPlayableCards();

		if(playableCards.empty()) return false;
		else {
			std::size_t chosenIndex = randomInRange<std::size_t>(0, playableCards.size() - 1);
			Card chosenCard = playableCards[chosenIndex];
			playCardImpl(chosenCard);
			return true;
		}
	}
	std::vector<Card> AiPlayer::getPlayableCards() {
		std::vector<Card> playableCards;
		for(auto& card : getHandCards()) {
			if(game.canPlay(*this, card)) {
				playableCards.push_back(card);
			}
		}
		return playableCards;
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