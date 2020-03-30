#include "MauMauGameAccessorFromClient.h"

#include <shared/packet/cts/MauRequest_CTSPacket.h>
#include <shared/packet/cts/PlayCardRequest_CTSPacket.h>
#include <shared/packet/cts/PassRequest_CTSPacket.h>
#include <shared/packet/cts/DrawCardRequest_CTSPacket.h>
#include <shared/model/MauMauCardValueMeanings.h>
#include <shared/model/CardAnimationDuration.h>

namespace card {
	MauMauGameAccessorFromClient::MauMauGameAccessorFromClient(ProxyMauMauGameData& gameData, std::shared_ptr<CTSPacketTransmitter> packetTransmitter) :
			gameData(gameData),
			packetTransmitter(packetTransmitter) {
	}
	bool MauMauGameAccessorFromClient::canMau() const {
		if(gameData.hasGameEnded() ||
		   !gameData.hasInitialCardBeenDistributed() ||
			gameData.getLocalPlayer()->wasMauDemandedThisTurn() ||
			! gameData.getOptions().getOption(Options::HAVE_TO_MAU)) {
			return false;
		}

		return true;
	}
	bool MauMauGameAccessorFromClient::canPlay(std::size_t indexInLocalPlayerHandCards) const {
		const CardAnimator& handCardStack = gameData.getLocalPlayer()->getCardStack();
		Card c = handCardStack.get(indexInLocalPlayerHandCards);
		return canPlay(c);
	}
	bool MauMauGameAccessorFromClient::canPlay(Card card) const {
		if(!gameData.isReadyToPerformLocalPlayerTurn() ||
		   gameData.getLocalPlayer()->hasPlayedCard() ||
		   gameData.getLocalPlayer()->hasTimeExpired()) {

			return false;
		}

		// TODO check: what happens if skipping/drawing two is disabled
		if(gameData.getLocalPlayer()->isInSkipState() && card.getValue() != SKIP_VALUE) return false;
		if(gameData.isInDrawTwoState() && card.getValue() != DRAW_2_VALUE) return false;

		auto& roomOptions = gameData.getOptions();
		const CardAnimator& playCardStack = gameData.getPlayStack();
		Card lastCardOnPlayStack = playCardStack.getLast();

		if(card.getValue() == CHANGE_COLOR_VALUE && roomOptions.getOption(Options::CHOOSE_COLOR_ON_JACK)) {
			if(!roomOptions.getOption(Options::CAN_PUT_JACK_ON_JACK) && lastCardOnPlayStack.getValue() == CHANGE_COLOR_VALUE) return false;
			if(roomOptions.getOption(Options::CAN_PUT_JACK_ON_EVERY_COLOR)) return true;
		}

		return card.getCardIndex() == gameData.getCardIndexForNextCard() || card.getValue() == lastCardOnPlayStack.getValue();
	}
	bool MauMauGameAccessorFromClient::canDraw() const {
		if(!gameData.isReadyToPerformLocalPlayerTurn() ||
		   isWaitingForColorChoose() ||
		   gameData.getLocalPlayer()->hasTimeExpired() ||
		   gameData.getLocalPlayer()->hasStartedToDrawCard() ||
		   canPass() ||
		   gameData.getDrawCardForNextPlayer() == Card::NULLCARD) {
			
			return false;
		}

		return true;
	}
	bool MauMauGameAccessorFromClient::canPlayDrawnCard() const {
		auto localPlayer = gameData.getLocalPlayer();

		if(! localPlayer->isCardInTemporaryStack()) return false;
		else return canPlay(* localPlayer->getCardInTempStack());
	}
	bool MauMauGameAccessorFromClient::canTakeDrawnCardIntoHandCards() const {
		auto localPlayer = gameData.getLocalPlayer();
		return localPlayer->isCardInTemporaryStack();
	}
	bool MauMauGameAccessorFromClient::isWaitingForColorChoose() const {
		return indexOfCardToPlayAfterColorChoose.has_value();
	}
	bool MauMauGameAccessorFromClient::canPass() const {
		auto localPlayer = gameData.getLocalPlayer();
		return gameData.isReadyToPerformLocalPlayerTurn() && (localPlayer->isInSkipState() || gameData.isInDrawTwoState());
	}
	void MauMauGameAccessorFromClient::mau() {
		if(! canMau()) throw std::runtime_error("Can't mau card in the current situation!");
		auto localPlayer = gameData.getLocalPlayer();
		localPlayer->onMauDemand();

		MauRequest_CTSPacket pkt;
		packetTransmitter->sendPacketToServer(pkt);
	}
	void MauMauGameAccessorFromClient::drawCard() {
		if(!canDraw()) throw std::runtime_error("Can't draw card in the current situation!");
		wasCardDrawnThisTurn = true;

		Card cardToDraw = gameData.getDrawCardForNextPlayer();
		if(canPlay(cardToDraw)) {
			// we don't have to send a packet yet, since we want to let the player choose if he wants
			// to add the drawn card into it's hand card or play it
			
			gameData.drawInLocalPlayerTempCards();
		} else {
			auto localPlayer = gameData.getLocalPlayer();
			gameData.drawInHandCardsFromCardStack(localPlayer, cardToDraw);
		
			DrawCardRequest_CTSPacket p;
			packetTransmitter->sendPacketToServer(p);
			
			gameData.setNextPlayerOnTurnLocal();
		}

		gameData.setDrawCardForNextPlayer(Card::NULLCARD);
	}
	void MauMauGameAccessorFromClient::takeDrawnCardIntoHandCards() {
		if(!canTakeDrawnCardIntoHandCards()) throw std::runtime_error("Can't pick drawn card!");
	
		gameData.drawInHandCardsFromTempCards();

		DrawCardRequest_CTSPacket p;
		packetTransmitter->sendPacketToServer(p);

		gameData.setNextPlayerOnTurnLocal();
	}

	void MauMauGameAccessorFromClient::playCard(std::size_t index) {
		auto localPlayer = gameData.getLocalPlayer();
		Card card = localPlayer->getCardStack().get(index);
		if(! canPlay(card)) throw std::runtime_error("Can't play card!");

		bool canChangeColor = gameData.canChangeColor(card);
		if(!canChangeColor || gameData.hasGameEnded()) {
			gameData.playCardFromLocalPlayerHandCards(index);
			sendPlayCardPacket();
		} else {
			indexOfCardToPlayAfterColorChoose = static_cast<int>(index);
		}
	}


	void MauMauGameAccessorFromClient::playDrawnCard() {
		if(! canPlayDrawnCard()) throw std::runtime_error("Can't play drawn card!");
		auto localPlayer = gameData.getLocalPlayer();
		Card card = *localPlayer->getCardInTempStack();

		bool canChangeColor = gameData.canChangeColor(card);
		if(!canChangeColor || gameData.hasGameEnded()) {
			gameData.playCardFromLocalPlayerTempCards();
			sendPlayCardPacket();
		} else {
			indexOfCardToPlayAfterColorChoose = INDEX_OF_CARD_IN_TEMP_STACK;
		}
	}


	void MauMauGameAccessorFromClient::pass() {
		if(! canPass()) throw std::runtime_error("Can't pass!");
		auto localPlayer = gameData.getLocalPlayer();
		PassRequest_CTSPacket p;
		packetTransmitter->sendPacketToServer(p);

		gameData.setNextPlayerOnTurnLocal();
	}

	void MauMauGameAccessorFromClient::chooseColor(CardIndex color) {
		if(!isWaitingForColorChoose()) throw std::runtime_error("Can't choose color!");
		auto localPlayer = gameData.getLocalPlayer();

		playPremarkedCardAfterColorChoose(color);
		sendPlayCardPacket(color);
	}

	void MauMauGameAccessorFromClient::playPremarkedCardAfterColorChoose(CardIndex newCardIndex) {
		auto localPlayer = gameData.getLocalPlayer();
		if(indexOfCardToPlayAfterColorChoose.has_value()) {
			std::optional<Card> drawnCard = localPlayer->getCardInTempStack();
			if(drawnCard.has_value()) {
				gameData.playCardFromLocalPlayerTempCards(newCardIndex);
			} else {
				int cardIndexToPlay = *indexOfCardToPlayAfterColorChoose;
				if(cardIndexToPlay == INDEX_OF_CARD_IN_TEMP_STACK) throw std::runtime_error("Can't play card in temp card stack from hand cards.");

				gameData.playCardFromLocalPlayerHandCards(cardIndexToPlay, newCardIndex);
			}
		}
	}

	void MauMauGameAccessorFromClient::sendPlayCardPacket(CardIndex newCardIndex) {
		auto localPlayer = gameData.getLocalPlayer();
		Card playedCard = *localPlayer->getPlayedCard();

		PlayCardRequest_CTSPacket p(playedCard.getCardNumber(), wasCardDrawnThisTurn, static_cast<int>(newCardIndex));
		packetTransmitter->sendPacketToServer(p);

		std::size_t cardsToDrawForNextPlayer = gameData.getAmountsOfCardsToDrawForNextPlayer(playedCard);
		gameData.addCardsToDrawOnPassDueToPlusTwo(cardsToDrawForNextPlayer);
		gameData.setNextPlayerOnTurnAndUpdateSkipAndDrawTwoState(playedCard);
	}

	void MauMauGameAccessorFromClient::onTurnEnd() {
		indexOfCardToPlayAfterColorChoose = std::nullopt;
		wasCardDrawnThisTurn = false;
	}


}