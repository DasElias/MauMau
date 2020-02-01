#include "ProxyMauMauGame.h"
#include <shared/model/PlayerNotFoundException.h>

#include <shared/model/MauMauCardValueMeanings.h>

#include <shared/packet/cts/MauRequest_CTSPacket.h>
#include <shared/packet/cts/PlayCardRequest_CTSPacket.h>
#include <shared/packet/cts/DrawCardRequest_CTSPacket.h>

#include <shared/packet/stc/OtherPlayerHasMauedSuccessfully_STCPacket.h>
#include <shared/packet/stc/MauPunishment_STCPacket.h>
#include <shared/packet/stc/TurnWasAborted_STCPacket.h>
#include <shared/packet/stc/OtherPlayerHasDrawnCards_STCPacket.h>
#include <shared/packet/stc/OtherPlayerHasPlayedCard_STCPacket.h>
#include <shared/packet/stc/LocalPlayerIsOnTurn_STCPacket.h>

#include <iostream>
#include <shared/model/CardAnimationDuration.h>
#include <shared/utils/ThreadUtils.h>
#include <shared/model/TimeToSetNextPlayerOnTurnDuration.h>
#include <shared/utils/Logger.h>

namespace card {
	ProxyMauMauGame::ProxyMauMauGame(std::shared_ptr<CTSPacketTransmitter> packetTransmitter, std::vector<std::shared_ptr<ParticipantOnClient>> allParticipantsInclLocal, std::shared_ptr<ParticipantOnClient> localParticipant, std::string usernameOnTurn, std::vector<int> handCards, int startCard, int nextCardOnDrawStack, RoomOptions& roomOptions) :
			packetTransmitter(packetTransmitter),
			drawCardStack(std::make_unique<CardStack>()),
			playCardStack(std::make_unique<CardStack>()),
			indexForNextCard(Card(startCard).getCardIndex()),
			roomOptions(roomOptions),
			winner(nullptr),
			handler_onOtherPlayerHasDrawnCard(std::bind(&ProxyMauMauGame::listener_onOtherPlayerHasDrawnCard, this, std::placeholders::_1)),
			handler_onOtherPlayerHasPlayedCard(std::bind(&ProxyMauMauGame::listener_onOtherPlayerHasPlayedCard, this, std::placeholders::_1)),
			handler_onLocalPlayerIsOnTurn(std::bind(&ProxyMauMauGame::listener_onLocalPlayerIsOnTurn, this, std::placeholders::_1)),
			handler_onTimeExpires(std::bind(&ProxyMauMauGame::listener_onTimeExpires, this, std::placeholders::_1)),
			handler_onMauPunishment(std::bind(&ProxyMauMauGame::listener_onMauPunishment, this, std::placeholders::_1)),
			handler_onOtherPlayerHasSuccessfullyMaued(std::bind(&ProxyMauMauGame::listener_onOtherPlayerHasSuccessfullyMaued, this, std::placeholders::_1)) {

		// initialize players
		for(auto& o : allParticipantsInclLocal) {
			if(o == localParticipant) {
				this->localPlayer = std::make_shared<LocalPlayer>(localParticipant, packetTransmitter, *this, gameInformation);
				this->allPlayers.push_back(localPlayer);
			} else {
				auto player = std::make_shared<ProxyPlayer>(o, gameInformation);
				this->opponents.push_back(player);
				this->allPlayers.push_back(player);
			}
		}

		// initialize other player's hand cards and draw card stack
		initStartCards(handCards, Card(startCard));

		// initialize player on turn
		this->userOnTurn = lookupPlayer(usernameOnTurn);
		threadUtils_invokeIn(getDurationUntilInitialCardsAreDistributed(allPlayers.size(), handCards.size()), [this]() {
			userOnTurn->onStartTurn();
			field_hasInitialCardsBeenDistributed = true;
		});
		setLocalPlayerAtTheBeginOfPlayersVector();

		// set first draw card
		if(isLocalPlayerOnTurn() && nextCardOnDrawStack == Card::NULLCARD.getCardNumber()) throw std::runtime_error("Can't set a NULLCARD as next card on draw card stack");
		this->drawCardForNextPlayer = Card(nextCardOnDrawStack);

		// initialize listeners
		packetTransmitter->addListenerForServerPkt(OtherPlayerHasDrawnCards_STCPacket::PACKET_ID, handler_onOtherPlayerHasDrawnCard);
		packetTransmitter->addListenerForServerPkt(OtherPlayerHasPlayedCard_STCPacket::PACKET_ID, handler_onOtherPlayerHasPlayedCard);
		packetTransmitter->addListenerForServerPkt(LocalPlayerIsOnTurn_STCPacket::PACKET_ID, handler_onLocalPlayerIsOnTurn);
		packetTransmitter->addListenerForServerPkt(TurnWasAborted_STCPacket::PACKET_ID, handler_onTimeExpires);
		packetTransmitter->addListenerForServerPkt(MauPunishment_STCPacket::PACKET_ID, handler_onMauPunishment);
		packetTransmitter->addListenerForServerPkt(OtherPlayerHasMauedSuccessfully_STCPacket::PACKET_ID, handler_onOtherPlayerHasSuccessfullyMaued);

	}
	
	ProxyMauMauGame::~ProxyMauMauGame() {
		packetTransmitter->removeListenerForServerPkt(OtherPlayerHasDrawnCards_STCPacket::PACKET_ID, handler_onOtherPlayerHasDrawnCard);
		packetTransmitter->removeListenerForServerPkt(OtherPlayerHasPlayedCard_STCPacket::PACKET_ID, handler_onOtherPlayerHasPlayedCard);
		packetTransmitter->removeListenerForServerPkt(LocalPlayerIsOnTurn_STCPacket::PACKET_ID, handler_onLocalPlayerIsOnTurn);
		packetTransmitter->removeListenerForServerPkt(TurnWasAborted_STCPacket::PACKET_ID, handler_onTimeExpires);
		packetTransmitter->removeListenerForServerPkt(MauPunishment_STCPacket::PACKET_ID, handler_onMauPunishment);
		packetTransmitter->removeListenerForServerPkt(OtherPlayerHasMauedSuccessfully_STCPacket::PACKET_ID, handler_onOtherPlayerHasSuccessfullyMaued);

	}

	void ProxyMauMauGame::initStartCards(const std::vector<int>& handCardNumbersOfLocalPlayer, Card cardOnPlayStack) {
		// init draw card stack
		drawCardStack.addFromPlain(Card::NULLCARD, Card::MAX_CARDS);

		std::size_t cardsPerPlayer = handCardNumbersOfLocalPlayer.size();
		std::vector<Card> nullHandCards = Card::getVectorWithCards(Card::NULLCARD, cardsPerPlayer);
		for(std::size_t i = 0; i < opponents.size(); i++) {
			auto& o = opponents[i];
			o->initHandCards(nullHandCards, drawCardStack, i);
		}
		
		std::vector<Card> handCardsOfLocalPlayer = Card::getVectorFromCardNumber(handCardNumbersOfLocalPlayer);
		localPlayer->initHandCards(handCardsOfLocalPlayer, drawCardStack, opponents.size());

		// init play card stack
		playCardStack.addLastCardFrom(cardOnPlayStack, drawCardStack, INITIAL_DRAW_DURATION_PLAYCARDSTACK_MS, getDelayUntilPlayStackCanBeInitialized(allPlayers.size(), handCardNumbersOfLocalPlayer.size()));
	}

	bool ProxyMauMauGame::isLocalPlayerOnTurn() const {
		return *userOnTurn == *localPlayer;
	}

	bool ProxyMauMauGame::areAllPreviousCardTransactionsCompleted() const {
		if(! playCardStack.getCardAnimations().empty()) return false;
		if(! localPlayer->getDrawnCardAsStack().getCardAnimations().empty()) return false;

		for(auto& player : this->allPlayers) {
			if(! player->getCardStack().getCardAnimations().empty()) return false;
		}
		
		return true;
	}

	bool ProxyMauMauGame::canPlay(std::size_t indexInLocalPlayerHandCards) const {
		return canPlay(localPlayer->getCardStack().get(indexInLocalPlayerHandCards));
	}

	bool ProxyMauMauGame::canPlay(Card card) const {
		if(! isLocalPlayerOnTurn() || !field_hasInitialCardsBeenDistributed || localPlayer->hasPlayedCard() || !areAllPreviousCardTransactionsCompleted() || hasGameEnded() || localPlayer->hasTimeExpired()) return false;

		Card lastCardOnPlayStack = playCardStack.getLast();
		if(lastCardOnPlayStack.getValue() == CHANGE_COLOR_VALUE) {
			// exception rule
			// can't play jack onto another jack card
			if(card.getValue() == CHANGE_COLOR_VALUE) return false;
		} else if(lastCardOnPlayStack.getCardIndex() != indexForNextCard) {
			throw std::runtime_error("Inconsistent model state!");
		}

		return card.getCardIndex() == indexForNextCard || card.getValue() == lastCardOnPlayStack.getValue();
	}

	bool ProxyMauMauGame::canDraw() const {
		if(! isLocalPlayerOnTurn() || !field_hasInitialCardsBeenDistributed || !areAllPreviousCardTransactionsCompleted() || isWaitingForColorChoose() || drawCardForNextPlayer == Card::NULLCARD || localPlayer->hasTimeExpired()) return false;
		if(hasGameEnded()) return false;

		// has already drawn a card
		if(localPlayer->hasStartedToDrawCard()) return false;

		return true;		
	}

	bool ProxyMauMauGame::canChangeColor(Card playedCard) const {
		return !hasGameEnded() && roomOptions.chooseCardIndexOnJack() && playedCard.getValue() == CHANGE_COLOR_VALUE;
	}

	bool ProxyMauMauGame::canMau() const {
		if(hasGameEnded()) return false;

		if(!field_hasInitialCardsBeenDistributed) return false;
		// player has already finished game, no more mau needed
		if(localPlayer->getCardStack().isEmptyAndNoPendingTransactions()) return false;

		if(field_wasMaued) return false;

		return true;
	}

	bool ProxyMauMauGame::canPlayDrawnCard() const {
		if(! localPlayer->isCardInTemporaryStack()) return false;
		return canPlay(* localPlayer->getDrawnCard());
	}

	bool ProxyMauMauGame::canTakeDrawnCardIntoHandCards() const {
		return localPlayer->isCardInTemporaryStack();
	}

	bool ProxyMauMauGame::canSkipPlayer(Card card) const {
		return roomOptions.skipOnEight() && card.getValue() == SKIP_VALUE;
	}

	bool ProxyMauMauGame::isWaitingForColorChoose() const {
		return field_isWaitingForColorChoose;
	}

	std::size_t ProxyMauMauGame::getAmountsOfCardsToDrawForNextPlayer(Card playedCard) const {
		if(! roomOptions.drawTwoCardsOnSeven()) return false;
		return (playedCard.getValue() == DRAW_2_VALUE) ? 2 : 0;
	}

	void ProxyMauMauGame::mau() {
		field_wasMaued = true;

		// TODO CHECK CONDITIONS
		MauRequest_CTSPacket pkt;
		packetTransmitter->sendPacketToServer(pkt);
	}

	void ProxyMauMauGame::drawCard() {
		if(! canDraw()) throw std::runtime_error("Can't draw card in the current situation!");
		if(drawCardForNextPlayer == Card::NULLCARD) throw std::runtime_error("Can't draw a NULLCARD");
		field_wasCardDrawn = true;

		if(canPlay(drawCardForNextPlayer)) {
			// we don't have to send a packet yet, since we want to let the player choose if he wants
			// to add the drawn card into it's hand card or play it

			localPlayer->drawSingleCardInTempCardStackLocal(drawCardForNextPlayer, drawCardStack);
		} else {
			localPlayer->drawSingleCardInHandCardsLocal(drawCardForNextPlayer, drawCardStack);

			DrawCardRequest_CTSPacket p;
			packetTransmitter->sendPacketToServer(p);
			setNextPlayerOnTurnLocal();
		}

		this->drawCardForNextPlayer = Card::NULLCARD;
	}

	void ProxyMauMauGame::takeDrawnCardIntoHandCards() {
		if(! canTakeDrawnCardIntoHandCards()) throw std::runtime_error("Can't pick drawn card!");

		localPlayer->sortDrawnCardIntoHandCardsLocal();

		DrawCardRequest_CTSPacket p;
		packetTransmitter->sendPacketToServer(p);

		setNextPlayerOnTurnLocal();
	}

	void ProxyMauMauGame::playCard(std::size_t index) {
		auto card = localPlayer->getCardStack().get(index);
		this->playCard(card);
	}

	void ProxyMauMauGame::playCard(Card card) {
		if(!canPlay(card)) throw std::runtime_error("Can't play card!");

		bool canChangeColor = this->canChangeColor(card);
		if(! canChangeColor || hasGameEnded()) {
			localPlayer->playCardFromHandCards(card, playCardStack);
			updateGameEndFlag();

			sendPlayCardPacket();
		} else {
			// else packet will be send in chooseColor()
			field_isWaitingForColorChoose = true;
			premarkedCardToPlayAfterColorChoose = card;
		}

	}

	void ProxyMauMauGame::playDrawnCard() {
		if(!canPlayDrawnCard()) throw std::runtime_error("Can't play drawn card!");

		Card drawnCard = *localPlayer->getDrawnCard();
		bool canChangeColor = this->canChangeColor(drawnCard);
		if(!canChangeColor || hasGameEnded()) {
			localPlayer->playCardFromTempCardStackLocal(playCardStack);
			updateGameEndFlag();

			sendPlayCardPacket();
		} else {
			// else packet will be send in chooseColor()
			field_isWaitingForColorChoose = true;
			premarkedCardToPlayAfterColorChoose = drawnCard;
		}
	}


	void ProxyMauMauGame::chooseColor(CardIndex color) {
		if(! isWaitingForColorChoose()) throw std::runtime_error("Can't choose color!");
		field_isWaitingForColorChoose = false;

		sendPlayCardPacket(color);
		this->indexForNextCard = color;
	}

	void ProxyMauMauGame::sendPlayCardPacket(CardIndex newCardIndex) {
		field_wasCardPlayed = true;
		if(premarkedCardToPlayAfterColorChoose.has_value()) {
			Card cardToPlay = *premarkedCardToPlayAfterColorChoose;

			std::optional<Card> drawnCard = localPlayer->getDrawnCard();
			if(drawnCard.has_value()) {
				if(*drawnCard != cardToPlay) throw std::runtime_error("Can't play card which isn't equal to the drawn card");
				localPlayer->playCardFromTempCardStackLocal(playCardStack);
			} else {
				localPlayer->playCardFromHandCards(cardToPlay, playCardStack);
			}
		}

		auto playedCard = *localPlayer->getPlayedCard();
		this->indexForNextCard = playedCard.getCardIndex();

		PlayCardRequest_CTSPacket p(playedCard.getCardNumber(), static_cast<int>(newCardIndex));
		packetTransmitter->sendPacketToServer(p);

		setNextOrNextButOneOnTurnLocal(playedCard);

		std::size_t cardsToDrawForNextPlayer = getAmountsOfCardsToDrawForNextPlayer(playedCard);
		playerHasToDrawCards(this->userOnTurn, cardsToDrawForNextPlayer, PLAY_DURATION_MS);

		this->drawCardForNextPlayer = Card::NULLCARD;
	}
	void ProxyMauMauGame::updateGameEndFlag() {
		for(auto& player : allPlayers) {
			if(player->getCardStack().isEmptyAndNoPendingTransactions()) {
				winner = player;
				break;
			}
		}
	}
	void ProxyMauMauGame::throwIfGameHasEnded() {
		if(hasGameEnded()) {
			throw std::runtime_error("Can't perform action when the game has already ended.");
		}
	}
	const CardAnimator& ProxyMauMauGame::getDrawStack() const {
		return drawCardStack;
	}

	const CardAnimator& ProxyMauMauGame::getPlayStack() const {
		return playCardStack;
	}

	std::shared_ptr<LocalPlayer> ProxyMauMauGame::getLocalPlayer() {
		return localPlayer;
	}

	std::vector<std::shared_ptr<ProxyPlayer>> ProxyMauMauGame::getOpponents() {
		return opponents;
	}

	void ProxyMauMauGame::removeOpponentLocal(std::shared_ptr<ParticipantOnClient> player) {
		if(hasGameEnded()) return;

		for(std::size_t i = 0; i < opponents.size(); i++) {
			if(opponents[i]->getWrappedParticipiant() == player) {
				opponents.erase(opponents.begin() + i);
				return;
			}
		}
		
		throw PlayerNotFoundException("Player \"" + player->getUsername() + "\" is not an opponent.");
	}

	bool ProxyMauMauGame::checkIfIsOpponent(std::string username) const {
		for(auto& o : opponents) {
			if(o->getUsername() == username) return true;
		}
		return false;
	}

	std::shared_ptr<ProxyPlayer> ProxyMauMauGame::lookupPlayer(std::string username) {
		for(auto& o : allPlayers) {
			if(o->getUsername() == username) return o;
		}

		throw PlayerNotFoundException("\"" + username + "\" is not a player.");
	}
	std::shared_ptr<ProxyPlayer> ProxyMauMauGame::lookupOpponent(std::string username) {
		for(auto& o : opponents) {
			if(o->getUsername() == username) return o;
		}

		throw PlayerNotFoundException("\"" + username + "\" is not an opponent.");
	}
	bool ProxyMauMauGame::hasGameEnded() const {
		return winner != nullptr;
	}
	std::shared_ptr<ProxyPlayer> ProxyMauMauGame::getWinnerOrNull() {
		return winner;
	}
	void ProxyMauMauGame::playCardAndSetNextPlayerOnTurnLocal(std::string username, Card card, CardIndex newCardIndex, std::vector<Card> cardsToDraw, bool wasDrawedJustBefore) {
		throwIfGameHasEnded();
		field_wasCardPlayed = true;

		std::shared_ptr<ProxyPlayer> player = lookupOpponent(username);

		if(wasDrawedJustBefore) {
			player->drawSingleCardInHandCardsLocal(card, drawCardStack);
			player->playCardFromHandCardsAfterDelay(card, playCardStack, DRAW_DURATION_MS + DELAY_BETWEEN_DRAW_AND_PLAY);
			field_wasCardDrawn = true;
		} else {
			player->playCardFromHandCards(card, playCardStack);
		}

		if(canChangeColor(card)) {
			if(newCardIndex == CardIndex::NULLINDEX) throw std::runtime_error("Can't set NULLINDEX as new card index");
			this->indexForNextCard = newCardIndex;
		} else {
			this->indexForNextCard = card.getCardIndex();
		}

		setNextOrNextButOneOnTurnLocal(card);

		int additionalDelayForDraw = (wasDrawedJustBefore) ? DRAW_DURATION_MS + DELAY_BETWEEN_DRAW_AND_PLAY : 0;
		playerHasToDrawCards(userOnTurn, cardsToDraw, PLAY_DURATION_MS + additionalDelayForDraw);
		updateGameEndFlag();
	}
	void ProxyMauMauGame::drawCardAndSetNextPlayerOnTurnLocal(std::string username) {
		throwIfGameHasEnded();
		field_wasCardDrawn = true;

		std::shared_ptr<ProxyPlayer> player = lookupOpponent(username);
		player->drawSingleCardInHandCardsLocal(Card::NULLCARD, drawCardStack);
		tryRebalanceCardStacks();

		setNextPlayerOnTurnLocal();
	}
	void ProxyMauMauGame::abortTurnOnTimeExpires(const std::vector<Card>& cardsToDraw) {
		if(isLocalPlayerOnTurn() && cardsToDraw[0] == Card::NULLCARD) {
			throw std::runtime_error("Local player can't draw NULLCARD on time expire!");
		}

		playerHasToDrawCards(userOnTurn, cardsToDraw);

		setNextPlayerOnTurnLocal();
		appendMessage("Zeit ist abgelaufen!");
	}
	void ProxyMauMauGame::onMauPunishment(std::string punishedUsername, std::vector<Card> cardsToDraw, MauPunishmentCause cause) {
		auto player = lookupPlayer(punishedUsername);
		int delay = (cause == MauPunishmentCause::NO_MAU_RECEIVED) ? PLAY_DURATION_MS : 0;
		playerHasToDrawCards(player, cardsToDraw, delay);

		appendMauPunishmentMessage(punishedUsername, cause);
	}
	void ProxyMauMauGame::appendMauPunishmentMessage(std::string punishedUsername, MauPunishmentCause cause) {
		switch(cause) {
			case MauPunishmentCause::TOO_EARLY:
				appendMessage(punishedUsername + u8" rief zu früh \"Mau!\"");
				break;
			case MauPunishmentCause::NO_MAU_RECEIVED:
				appendMessage(punishedUsername + u8" vergaß, \"Mau!\" zu rufen!");
				break;
			case MauPunishmentCause::DRAWED_EVEN_THOUGH_MAUED:
				appendMessage(punishedUsername + " spielte keine Karte, obwohl er \"Mau!\" rief.");
				break;
			default:
				log(LogSeverity::WARNING, "Unknown MauPunishmentCause: " + std::to_string(static_cast<int>(cause)));
		}
	}
	void ProxyMauMauGame::setNextOrNextButOneOnTurnLocal(Card playedCard) {
		if(canSkipPlayer(playedCard)) setNextButOnePlayerOnTurnLocal();
		else setNextPlayerOnTurnLocal();
	}
	void ProxyMauMauGame::setNextPlayerOnTurnLocal() {
		auto playerOnTurnIter = std::find(allPlayers.begin(), allPlayers.end(), userOnTurn);
		playerOnTurnIter++;
		if(playerOnTurnIter == allPlayers.end()) playerOnTurnIter = allPlayers.begin();

		setOnTurnLocal(*playerOnTurnIter);
	}
	void ProxyMauMauGame::setNextButOnePlayerOnTurnLocal() {
		auto playerOnTurnIter = std::find(allPlayers.begin(), allPlayers.end(), userOnTurn);

		playerOnTurnIter++;
		if(playerOnTurnIter == allPlayers.end()) playerOnTurnIter = allPlayers.begin();
		(* playerOnTurnIter)->onSkip();

		playerOnTurnIter++;
		if(playerOnTurnIter == allPlayers.end()) playerOnTurnIter = allPlayers.begin();

		setOnTurnLocal(*playerOnTurnIter);
	}
	void ProxyMauMauGame::setOnTurnLocal(std::shared_ptr<ProxyPlayer> player) {	
		int delayToSetNextPlayerOnTurn = getTimeToSetNextPlayerOnTurn(playCardStack.getSizeInclPendingTransactions(), playCardStack.getLastInclAnimations(), field_wasCardPlayed, field_wasCardDrawn);
		int delayToFreezeAnimation = getTimeToEndCurrentTurn(playCardStack.getSizeInclPendingTransactions(), playCardStack.getLastInclAnimations(), field_wasCardPlayed, field_wasCardDrawn);
		// TODO FIX: Wenn Bedingung erfüllt, delayToFreezeAnimation von delayToSetNextPlayerOnTurn abziehen
		if(field_wasCardDrawn && field_wasCardPlayed && userOnTurn == localPlayer) {
			// we don't have to take the time for drawing the card into consideration
			delayToFreezeAnimation = 0;
		}


		std::shared_ptr<ProxyPlayer> lastUserOnTurn = this->userOnTurn;

		this->userOnTurn->onEndTurn();
		this->userOnTurn = player;

		field_wasCardDrawn = false;
		field_wasCardPlayed = false;
		field_isWaitingForColorChoose = false;
		field_wasMaued = false;
		premarkedCardToPlayAfterColorChoose = std::nullopt;
		threadUtils_invokeIn(delayToSetNextPlayerOnTurn, [this, player, lastUserOnTurn]() {			
			lastUserOnTurn->endRemainingTimeAnimation();
			this->userOnTurn->onStartTurn();
		});
		threadUtils_invokeIn(delayToFreezeAnimation, [this, lastUserOnTurn]() {
			lastUserOnTurn->freezeRemainingTimeAnimation();
		});
	}

	std::optional<CardIndex> ProxyMauMauGame::getCardIndexForNextCardOrNone() const {
		if(playCardStack.getSize() <= 1 || playCardStack.getLast().getValue() != CHANGE_COLOR_VALUE) return std::nullopt;
		else return indexForNextCard;
	}
	
	void ProxyMauMauGame::playerHasToDrawCards(std::shared_ptr<ProxyPlayer> player, std::size_t amountOfCards, int delayMs) {
		if(amountOfCards == 0) return;

		auto cards = Card::getVectorWithCards(Card::NULLCARD, amountOfCards);
		playerHasToDrawCards(player, cards, delayMs);
	}

	void ProxyMauMauGame::playerHasToDrawCards(std::shared_ptr<ProxyPlayer> player, const std::vector<Card>& cards, int delayMs) {
		player->drawMultipleCardsInHandCardsAfterDelay(cards, drawCardStack, delayMs);
		tryRebalanceCardStacks();
	}

	void ProxyMauMauGame::tryRebalanceCardStacks() {
		if(drawCardStack.getSize() <= 3 && playCardStack.getSize() >= 1) {
			playCardStack.addFirstCardFrom(Card::NULLCARD, drawCardStack, REBALANCE_DURATION, 0);
		}
	}
	void ProxyMauMauGame::setLocalPlayerAtTheBeginOfPlayersVector() {
		while(this->allPlayers[0] != localPlayer) {
			allPlayers.push_back(allPlayers[0]);
			allPlayers.erase(allPlayers.begin());

			opponents.push_back(opponents[0]);
			opponents.erase(opponents.begin());
		}
	}
	void ProxyMauMauGame::appendMessage(std::string content) {
		messageQueue.appendMessage(content);
	}
	const MessageQueue ProxyMauMauGame::getMessageQueue() const {
		return messageQueue;
	}
	void ProxyMauMauGame::listener_onOtherPlayerHasDrawnCard(Packet& p) {
		auto& casted = dynamic_cast<OtherPlayerHasDrawnCards_STCPacket&>(p);
		drawCardAndSetNextPlayerOnTurnLocal(casted.getUsername());
	}
	void ProxyMauMauGame::listener_onOtherPlayerHasPlayedCard(Packet& p) {
		auto& casted = dynamic_cast<OtherPlayerHasPlayedCard_STCPacket&>(p);
		playCardAndSetNextPlayerOnTurnLocal(casted.getUsername(), Card(casted.getCardNumber()), static_cast<CardIndex>(casted.getNewCardIndex()), Card::getVectorFromCardNumber(casted.getCardsToDraw()), casted.wasDrawnBeforePlayed());
	}
	void ProxyMauMauGame::listener_onLocalPlayerIsOnTurn(Packet& p) {
		auto& casted = dynamic_cast<LocalPlayerIsOnTurn_STCPacket&>(p);
		Card nextCardOnDrawStack(casted.getNextCardOnDrawStack());
		if(nextCardOnDrawStack == Card::NULLCARD) throw std::runtime_error("Can't set NULLCARD as draw card for next player.");

		this->drawCardForNextPlayer = nextCardOnDrawStack;
	}
	void ProxyMauMauGame::listener_onTimeExpires(Packet& p) {
		auto& casted = dynamic_cast<TurnWasAborted_STCPacket&>(p);
		std::vector<int> cardNumbersToDraw = casted.getCardsToDraw();
		std::vector<Card> cardsToDraw = Card::getVectorFromCardNumber(cardNumbersToDraw);

		abortTurnOnTimeExpires(cardsToDraw);

	}
	void ProxyMauMauGame::listener_onMauPunishment(Packet& p) {
		auto& casted = dynamic_cast<MauPunishment_STCPacket&>(p);
		std::vector<int> cardNumbersToDraw = casted.getCardsToDraw();
		std::vector<Card> cardsToDraw = Card::getVectorFromCardNumber(cardNumbersToDraw);

		onMauPunishment(casted.getConcernedUsername(), cardsToDraw, casted.getCause());
	}
	void ProxyMauMauGame::listener_onOtherPlayerHasSuccessfullyMaued(Packet& p) {
		auto& casted = dynamic_cast<OtherPlayerHasMauedSuccessfully_STCPacket&>(p);
		auto player = lookupPlayer(casted.getUsername());
		player->onMau();
	}
}
