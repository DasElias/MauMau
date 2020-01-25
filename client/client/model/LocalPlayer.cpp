#include "LocalPlayer.h"
#include <stdexcept>

#include <shared/utils/Logger.h>

#include <shared/packet/cts/MauRequest_CTSPacket.h>
#include <shared/packet/cts/PlayCardRequest_CTSPacket.h>
#include <shared/packet/cts/DrawCardRequest_CTSPacket.h>
#include <shared/model/CardAnimationDuration.h>

namespace card {
	LocalPlayer::LocalPlayer(std::shared_ptr<ParticipantOnClient> wrappedLocalPlayer, std::shared_ptr<CTSPacketTransmitter> packetTransmitter, PlayVerifier& playVerifier, ProxyPlayerGameInformation& gameInformation) :
			ProxyPlayer(wrappedLocalPlayer, gameInformation),
			wrappedLocalPlayer(wrappedLocalPlayer),
			packetTransmitter(packetTransmitter),
			playVerifier(playVerifier),
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

	void LocalPlayer::playCardFromHandCardsAfterDelay(Card card, CardAnimator& playCardStack, int delayMs) {
		this->playedCard = card;
		playCardStack.addDeterminedCardFrom(card, handCardStack, PLAY_DURATION_MS, delayMs);
	}

	void LocalPlayer::playCardFromHandCards(Card card, CardAnimator& playCardStack) {
		this->playedCard = card;
		playCardStack.addDeterminedCardFromImmediately(card, handCardStack, PLAY_DURATION_MS);
	}

	void LocalPlayer::playCardFromTempCardStackLocal(CardAnimator& playCardStack) {
		if(!isCardInTemporaryStack()) throw std::runtime_error("The player hasn't drawn a card yet!");
		this->playedCard = getDrawnCard();
		playCardStack.addDeterminedCardFromImmediately(*getDrawnCard(), drawnCardTempStack, PLAY_DURATION_MS);
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

	std::optional<Card> LocalPlayer::getDrawnCard() const {
		if(drawnCardTempStack.isEmpty()) return std::nullopt;
		else if(drawnCardTempStack.getSize() == 1) return drawnCardTempStack.get(0);
		else throw std::runtime_error("drawnCardTempStack contains more than one card!");
	}

	const CardAnimator& LocalPlayer::getDrawnCardAsStack() const {
		return drawnCardTempStack;
	}

	void LocalPlayer::onStartTurn() {
		ProxyPlayer::onStartTurn();

		//TODO: REMOVE INCORRECT COMMENT
		// we clear the temporary draw stack only when the player is on turn again
		// this is because of threading issues (the card is moved out of the temporary
		// card stack in the next frame, so during rendering we would have the situation that
		// the flag is false but the card stack isn't empty
	

		log(LogSeverity::DEBUG, "LocalPlayer is on turn.");
	}

	void LocalPlayer::onEndTurn() {
		ProxyPlayer::onEndTurn();

		playedCard = std::nullopt;
		drawnCardTempStack.clear();
		wasCardDrawn_flag = false;
		
		log(LogSeverity::DEBUG, "Turn has ended");
		
	}
}