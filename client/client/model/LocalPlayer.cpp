#include "LocalPlayer.h"
#include <stdexcept>

#include <shared/utils/Logger.h>

#include <shared/packet/cts/MauRequest_CTSPacket.h>
#include <shared/packet/cts/PlayCardRequest_CTSPacket.h>
#include <shared/packet/cts/DrawCardRequest_CTSPacket.h>
#include <shared/model/CardAnimationDuration.h>

namespace card {
	LocalPlayer::LocalPlayer(std::shared_ptr<ParticipantOnClient> wrappedLocalPlayer, std::shared_ptr<CTSPacketTransmitter> packetTransmitter, PlayVerifier& playVerifier) :
			ProxyPlayer(wrappedLocalPlayer),
			wrappedLocalPlayer(wrappedLocalPlayer),
			packetTransmitter(packetTransmitter),
			playVerifier(playVerifier),
			drawnCardTempStack(std::make_unique<HandCardStack>()) {

	//	if(handCards.empty()) throw std::runtime_error("LocalPlayer needs at least one default hand card");
//		this->selectedCard = handCards.at((handCards.size() - 1) / 2);

	}

	void LocalPlayer::drawCardLocal(Card mutatesTo, CardAnimator& source) {
		if(hasStartedToDrawCard()) {
			log(LogSeverity::ERR, "Inconsistent data model. Card was moved to LocalPlayer even though he has already got a card.");
		} else {
			drawnCardTempStack.addLastCardFromImmediately(mutatesTo, source, DRAW_DURATION_MS);
			wasCardDrawn_flag = true;
		}
	}

	void LocalPlayer::playCardLocal(Card card, CardAnimator& destination, bool isWaitingForColorPick_local) {
		this->playedCard = card;

		if(isWaitingForColorPick_local) {
			this->isWaitingForColorPick_field = isWaitingForColorPick_local;
		}

		if(! isCardInTemporaryStack()) {
			// player hasn't drawn a card, which could be played
			destination.addDeterminedCardFromImmediately(card, handCardStack, PLAY_DURATION_MS);
		} else {
			if(drawnCardTempStack.getSize() > 1 || ! (card == drawnCardTempStack.get(0))) {
				throw std::runtime_error("Inconsistent data model. There are too many cards on drawnCardTempStack or the player tries to play a card which wasn't drawn by him.");
			} 

			destination.addDeterminedCardFromImmediately(card, drawnCardTempStack, PLAY_DURATION_MS);
		}
	}

	std::optional<Card> LocalPlayer::getPlayedCard() const {
		return playedCard;
	}

	bool LocalPlayer::hasPlayedCard() const {
		return getPlayedCard().has_value();
	}

	void LocalPlayer::sortDrawnCardIntoHandCardsLocal() {
		if(! isCardInTemporaryStack()) throw std::runtime_error("The player hasn't drawn a card yet!");
		ProxyPlayer::drawCardLocal(drawnCardTempStack.get(0), drawnCardTempStack);
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

	void LocalPlayer::clearWaitingForColorPickFlag() {
		this->isWaitingForColorPick_field = false;
	}

	bool LocalPlayer::isWaitingForColorPick() const {
		return this->isWaitingForColorPick_field;
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
		isWaitingForColorPick_field = false;
		for(int counter = static_cast<int>(drawnCardTempStack.getSize() - 1); counter > 0; counter--) {
			ProxyPlayer::drawCardLocal(drawnCardTempStack.get(counter), drawnCardTempStack);
		}
		wasCardDrawn_flag = false;
		
		log(LogSeverity::DEBUG, "Turn has ended");
		
	}
}