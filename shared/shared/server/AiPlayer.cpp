#include "AiPlayer.h"
#include "../utils/MathUtils.h"
#include "../utils/ThreadUtils.h"
#include <climits>
#include "../model/MauMauCardValueMeanings.h"
#include "../model/CardAnimationDuration.h"
#include "../model/TimeToSetNextPlayerOnTurnDuration.h"
#include "../utils/Logger.h"

namespace card {
	float const AiPlayer::MAU_MISS_PROBABILITY = 0.1f;

	AiPlayer::AiPlayer(std::shared_ptr<ParticipantOnServer> wrappedParticipant, ServerMauMauGame& game, std::vector<Card> handCards) :
			Player(wrappedParticipant, handCards),
			game(game) {
	}
	AiPlayer::~AiPlayer() {
		threadUtils_removeCallbacksWithKey(this);
	}
	void AiPlayer::onStartTurn() {
		Player::onStartTurn();

		int delay = randomInRange(1250, 1750);
		auto& playCardStack = game.getPlayCardStack();
		delay += getTimeToSetNextPlayerOnTurn(playCardStack.getSize(), playCardStack.getLast(), game.wasCardPlayedLastTurn(), game.wasCardDrawnLastTurn(), game.getOptions());
		threadUtils_invokeIn(delay, this, [this]() {
			performTurn();
		});
	}
	void AiPlayer::performTurn() {
		if(game.isInSkipState()) {
			playIfIsInSkipState();
			return;
		}
		if(tryPlayCard()) return;

		// player cannot play, therefore no card was played
		if(shouldPlayDrawnCard()) {
			Card drawnCard = game.getDrawCardStack().getLast();
			playCardImpl(drawnCard, true);
		} else {
			drawCardImpl();
		}
	}
	
	bool AiPlayer::tryPlayCard() {
		std::vector<Card> playableCards = getPlayableCards();

		if(playableCards.empty()) return false;
		else {
			tryMau();

			std::size_t chosenIndex = randomInRange<std::size_t>(0, playableCards.size() - 1);
			Card chosenCard = playableCards[chosenIndex];
			playCardImpl(chosenCard, false);
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
	void AiPlayer::tryMau() {
		if(game.canMau(*this)) {
			if(randomReal<float>() > MAU_MISS_PROBABILITY) {
				game.mau(*this);
			}
		}
	}
	void AiPlayer::playCardImpl(Card card, bool wasCardJustDrawn) {
		CardIndex nextCardIndex = (game.canChangeColor(card)) ? chooseCardIndex(card.getCardIndex()) : CardIndex::NULLINDEX;
		bool success = game.playCardAndSetNextPlayerOnTurn(*this, card, wasCardJustDrawn, nextCardIndex);
		if(!success) {
			log(LogSeverity::ERR, "AiPlayer " + getUsername() + " couldn't play card!");
		}
	}
	void AiPlayer::drawCardImpl() {
		bool success = game.drawCardAndSetNextPlayerOnTurn(*this);
		if(!success) {
			log(LogSeverity::ERR, "AiPlayer " + getUsername() + " couldn't draw card!");
		}
	}
	CardIndex AiPlayer::chooseCardIndex(CardIndex indexOfPlayedJack) {
		std::map<CardIndex, uint8_t> cardIndexCounter;
		cardIndexCounter[CardIndex::HEART] = 0;
		cardIndexCounter[CardIndex::CLUB] = 0;
		cardIndexCounter[CardIndex::DIAMOND] = 0;
		cardIndexCounter[CardIndex::SPADE] = 0;
		for(auto& card : getHandCards()) {
			if(cardIndexCounter.find(card.getCardIndex()) != cardIndexCounter.end()) {
				cardIndexCounter[card.getCardIndex()]++;
			} else {
				log(LogSeverity::WARNING, "AiPlayer has a NULLCARD in it's hand cards.");
			}
		}

		std::vector<std::pair<CardIndex, uint8_t>> cardIndexList;
		cardIndexList.push_back({CardIndex::HEART, cardIndexCounter[CardIndex::HEART]});
		cardIndexList.push_back({CardIndex::CLUB, cardIndexCounter[CardIndex::CLUB]});
		cardIndexList.push_back({CardIndex::DIAMOND, cardIndexCounter[CardIndex::DIAMOND]});
		cardIndexList.push_back({CardIndex::SPADE, cardIndexCounter[CardIndex::SPADE]});

		std::sort(cardIndexList.begin(), cardIndexList.end(), [](const auto& a, const auto& b) {
			return a.second > b.second;
		});

		for(auto& cardIndexEntry : cardIndexList) {
			if(cardIndexEntry.first != indexOfPlayedJack) return cardIndexEntry.first;
		}
		return CardIndex::NULLINDEX;
	}
	bool AiPlayer::shouldPlayDrawnCard() {
		Card lastCardOnDrawStack = game.getDrawCardStack().getLast();
		return game.canPlay(*this, lastCardOnDrawStack);
	}
	void AiPlayer::playIfIsInSkipState() {
		// we can't use getPlayableCard() since ServerMauMauGame::canPlay returns always
		// false if the game is in skip state
		std::vector<Card> playableCards;	
		for(auto& c : getHandCards()) {
			if(c.getValue() == SKIP_VALUE) playableCards.push_back(c);
		}

		bool success;
		if(playableCards.empty()) {
			success = game.pass(*this);
		} else {
			std::size_t randomIndex = randomInRange<std::size_t>(0, playableCards.size() - 1);
			Card cardToPlay = playableCards[randomIndex];
			success = game.playCardAndSetNextPlayerOnTurn(*this, cardToPlay, false);
		}

		if(!success) {
			log(LogSeverity::ERR, "AiPlayer " + getUsername() + " couldn't play card!");
		}
	}
}