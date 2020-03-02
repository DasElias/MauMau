#include "LocalPlayer.h"
#include <stdexcept>

#include <shared/utils/Logger.h>

#include <shared/packet/cts/MauRequest_CTSPacket.h>
#include <shared/packet/cts/PlayCardRequest_CTSPacket.h>
#include <shared/packet/cts/DrawCardRequest_CTSPacket.h>
#include <shared/model/CardAnimationDuration.h>

namespace card {
	LocalPlayer::LocalPlayer(std::shared_ptr<ParticipantOnClient> wrappedLocalPlayer,  ProxyPlayerGameInformation& gameInformation) :
			ProxyPlayer(wrappedLocalPlayer, gameInformation),
			wrappedLocalPlayer(wrappedLocalPlayer),
			drawnCardTempStack(std::make_unique<HandCardStack>()) {

	//	if(handCards.empty()) throw std::runtime_error("LocalPlayer needs at least one default hand card");
//		this->selectedCard = handCards.at((handCards.size() - 1) / 2);

	}

	std::optional<Card> LocalPlayer::getPlayedCard() const {
		return playedCard;
	}

	bool LocalPlayer::hasPlayedCard() const {
		return getPlayedCard().has_value();
	}

	void LocalPlayer::drawSingleCardInTempCardStackLocal(Card mutatesTo, CardAnimator& drawCardStack) {
		if(hasStartedToDrawCard()) {
			log(LogSeverity::ERR, "Inconsistent data model. Card was moved to LocalPlayer even though he has already got a card.");
		} else {
			drawnCardTempStack.addLastCardFromImmediately(mutatesTo, drawCardStack, DRAW_DURATION_MS);
			wasCardDrawn_flag = true;
		}
	}

	void LocalPlayer::playCardFromHandCards(Card card, CardAnimator& playCardStack) {
		std::size_t index = handCardStack.find(card);
		playCardFromHandCards(index, playCardStack);
	}

	void LocalPlayer::playCardFromHandCardsAfterDelay(Card card, CardAnimator& playCardStack, int delayMs) {
		std::size_t index = handCardStack.find(card);
		playCardFromHandCardsAfterDelay(index, playCardStack, delayMs);
	}

	void LocalPlayer::playCardFromHandCardsAfterDelay(std::size_t indexInHandCards, CardAnimator& playCardStack, int delayMs) {
		Card card = handCardStack.get(indexInHandCards);
		this->playedCard = card;
		playCardStack.addDeterminedCardFrom(indexInHandCards, handCardStack, PLAY_DURATION_MS, delayMs);
	}

	void LocalPlayer::playCardFromHandCards(std::size_t indexInHandCards, CardAnimator& playCardStack) {
		Card card = handCardStack.get(indexInHandCards);
		this->playedCard = card;
		playCardStack.addDeterminedCardFromImmediately(indexInHandCards, handCardStack, PLAY_DURATION_MS);
	}

	void LocalPlayer::playCardFromTempCardStackLocal(CardAnimator& playCardStack) {
		if(!isCardInTemporaryStack()) throw std::runtime_error("The player hasn't drawn a card yet!");
		this->playedCard = getCardInTempStack();
		playCardStack.addDeterminedCardFromImmediately(0, drawnCardTempStack, PLAY_DURATION_MS);
	}

	void LocalPlayer::sortDrawnCardIntoHandCardsLocal() {
		if(! isCardInTemporaryStack()) throw std::runtime_error("The player hasn't drawn a card yet!");
		drawSingleCardInHandCardsLocal(drawnCardTempStack.get(0), drawnCardTempStack);
	}

	bool LocalPlayer::hasStartedToDrawCard() const {
		if(!wasCardDrawn_flag && !drawnCardTempStack.isEmptyAndNoPendingTransactions()) throw std::runtime_error("Inconsistent data model. wasCardDrawn_flag cannot be false when the player has drawn a card");
		return wasCardDrawn_flag;
	}

	bool LocalPlayer::isCardInTemporaryStack() const {
		if(!wasCardDrawn_flag && !drawnCardTempStack.isEmptyAndNoPendingTransactions()) throw std::runtime_error("Inconsistent data model. wasCardDrawn_flag cannot be false when the player has drawn a card");
		return ! drawnCardTempStack.isEmpty();
	}

	std::optional<Card> LocalPlayer::getCardInTempStack() const {
		if(drawnCardTempStack.isEmpty()) return std::nullopt;
		else if(drawnCardTempStack.getSize() == 1) return drawnCardTempStack.get(0);
		else throw std::runtime_error("drawnCardTempStack contains more than one card!");
	}

	const CardAnimator& LocalPlayer::getTempCardStack() const {
		return drawnCardTempStack;
	}

	bool LocalPlayer::wasMauDemandedThisTurn() const {
		return wasMauDemandedThisTurn_flag;
	}

	void LocalPlayer::onMauDemand() {
		wasMauDemandedThisTurn_flag = true;
	}

	void LocalPlayer::onMauFailure() {
		wasMauDemandedThisTurn_flag = false;
	}

	void LocalPlayer::onEndTurn() {
		ProxyPlayer::onEndTurn();

		playedCard = std::nullopt;
		drawnCardTempStack.clear();
		wasCardDrawn_flag = false;
		wasMauDemandedThisTurn_flag = false;		
	}
}