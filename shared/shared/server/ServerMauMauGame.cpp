#include "ServerMauMauGame.h"
#include "AiPlayer.h"

#include "../packet/cts/MauRequest_CTSPacket.h"
#include "../packet/cts/DrawCardRequest_CTSPacket.h"
#include "../packet/cts/PlayCardRequest_CTSPacket.h"
#include "../packet/cts/PassRequest_CTSPacket.h"

#include "../packet/stc/InitialPlayerIsOnTurn_STCPacket.h"
#include "../packet/stc/PlayerHasMauedSuccessfully_STCPacket.h"
#include "../packet/stc/MauPunishment_STCPacket.h"
#include "../packet/stc/TurnWasAborted_STCPacket.h"
#include "../packet/stc/LocalPlayerIsOnTurn_STCPacket.h"
#include "../packet/stc/OtherPlayerHasDrawnCards_STCPacket.h"
#include "../packet/stc/OtherPlayerHasPlayedCard_STCPacket.h"
#include "../packet/stc/OtherPlayerHasPassed_STCPacket.h"
#include "../packet/stc/GameHasBeenStarted_STCPacket.h"
#include "../packet/stc/GameWasAborted_STCPacket.h"
#include "../model/PlayerNotFoundException.h"
#include "../model/MauMauCardValueMeanings.h"
#include "../utils/Logger.h"
#include <iostream>

#include "../utils/MathUtils.h"
#include "../utils/ThreadUtils.h"
#include "../model/CardAnimationDuration.h"
#include "../model/TimeToSetNextPlayerOnTurnDuration.h"
#include "../model/MaxTurnDuration.h"
#include "../model/MinDrawCardStackSize.h"

namespace card {
	uint64_t ServerMauMauGame::startTurnAbortIdCounter = 0;

	ServerMauMauGame::ServerMauMauGame(std::shared_ptr<STCPacketTransmitter> packetTransmitter, ServerGameEndHandler& gameEndHandler, std::vector<std::shared_ptr<ParticipantOnServer>> participants, RoomOptions& options) :
			packetTransmitter(packetTransmitter),
			gameEndHandler(gameEndHandler),
			roomOptions(options),
			allPlayers({}, nullptr),	// will be initialized later
			drawCardStack(),
			playCardStack(),
			handler_onPlayCard(std::bind(&ServerMauMauGame::listener_onPlayCard, this, std::placeholders::_1, std::placeholders::_2)),
			handler_onDrawCard(std::bind(&ServerMauMauGame::listener_onDrawCard, this, std::placeholders::_1, std::placeholders::_2)),
			handler_onMau(std::bind(&ServerMauMauGame::listener_onMau, this, std::placeholders::_1, std::placeholders::_2)),
			handler_onPass(std::bind(&ServerMauMauGame::listener_onPass, this, std::placeholders::_1, std::placeholders::_2)) {

		for(int i = 0; i < options.getOption(Options::AMOUNT_OF_START_CARD_DECKS); i++) {
			addCardDeckToDrawStack();
		}
		drawCardStack.shuffle();

		// init play card stack
		Card firstCardOnPlayStack;
		for(std::size_t i = 0; i < drawCardStack.getSize(); i++) {
			auto card = drawCardStack.get(i);
			if(!canChangeColor(card) && getAmountsOfCardsToDrawForNextPlayer(card) == 0 && !canSkipPlayer(card) && !canChangeDirection(card)) {
				firstCardOnPlayStack = card;

				drawCardStack.remove(i);
				playCardStack.addFromPlain(card);
				break;
			}
		}

		this->indexForNextCard = firstCardOnPlayStack.getCardIndex();

		// init players
		for(auto& p : participants) {
			auto constructedPlayer = (p->isRealPlayer()) ? std::make_shared<Player>(p) : std::make_shared<AiPlayer>(p, *this);
			allPlayers.appendPlayer(constructedPlayer);

			int startCardsPerPlayer = options.getOption(Options::AMOUNT_OF_START_CARDS);
			for(int i = 0; i < startCardsPerPlayer; i++) {
				Card removed = drawCardStack.removeLast();
				constructedPlayer->addHandCard(removed);
			}
		}

		// set player on turn
		setInitialPlayerOnTurn();

		// init packet listeners
		packetTransmitter->addListenerForClientPkt(PlayCardRequest_CTSPacket::PACKET_ID, handler_onPlayCard);
		packetTransmitter->addListenerForClientPkt(DrawCardRequest_CTSPacket::PACKET_ID, handler_onDrawCard);
		packetTransmitter->addListenerForClientPkt(MauRequest_CTSPacket::PACKET_ID, handler_onMau);
		packetTransmitter->addListenerForClientPkt(PassRequest_CTSPacket::PACKET_ID, handler_onPass);
	}
	
	ServerMauMauGame::~ServerMauMauGame() {
		packetTransmitter->removeListenerForClientPkt(PlayCardRequest_CTSPacket::PACKET_ID, handler_onPlayCard);
		packetTransmitter->removeListenerForClientPkt(DrawCardRequest_CTSPacket::PACKET_ID, handler_onDrawCard);
		packetTransmitter->removeListenerForClientPkt(MauRequest_CTSPacket::PACKET_ID, handler_onMau);
		packetTransmitter->removeListenerForClientPkt(PassRequest_CTSPacket::PACKET_ID, handler_onPass);

		// all threadUtils callbacks for this object aren't executed after the object is destroyed
		threadUtils_removeCallbacksWithKey(this);

		// we increment this counter to prevent that the turn abort callback for the player currently on turn is called
		startTurnAbortIdCounter++;
	}

	void ServerMauMauGame::setInitialPlayerOnTurn() {
		int startCardsPerPlayer = roomOptions.getOption(Options::AMOUNT_OF_START_CARDS);
		int timeUntilCardsDistributed = getDurationUntilInitialCardsAreDistributed(allPlayers.getAmountOfPlayers(), startCardsPerPlayer);
		threadUtils_invokeIn(timeUntilCardsDistributed, this, [this]() {
			auto& newPlayerOnTurn = allPlayers.getRandomPlayer();
			allPlayers.setPlayerOnTurn(newPlayerOnTurn);

			newPlayerOnTurn.onStartTurn();
			startTurnAbortTimer();

			for(auto& p : allPlayers) {
				Card nextCardOnDrawStack = (newPlayerOnTurn == p) ? drawCardStack.getLast() : Card::NULLCARD;
				InitialPlayerIsOnTurn_STCPacket packet(newPlayerOnTurn.getUsername(), nextCardOnDrawStack.getCardNumber());
				packetTransmitter->sendPacketToClient(packet, p.getWrappedParticipant());
			}
		});
	}

	void ServerMauMauGame::mau(Player& player) {
		if(! roomOptions.getOption(Options::HAVE_TO_MAU)) return;

		if(!canMau(player)) {
			sendMauPunishmentPacket(player, MauPunishmentCause::TOO_EARLY);
			return;
		}
		if(wasMauedCorrectly_thisTurn) {
			// not a critical error, therefore we only display a warning
			log(LogSeverity::WARNING, "Player " + player.getUsername() + " has maued twice.");
			return;
		}

		wasMauedCorrectly_thisTurn = true;
		sendSuccessfulMauPacket(player);
	}

	bool ServerMauMauGame::playCardAndSetNextPlayerOnTurn(Player& player, Card card, bool wasCardJustDrawn, CardIndex chosenIndex) {
		if(!canPlay(player, card)) {
			log(LogSeverity::ERR, "Player " + player.getUsername() + " tried to play card " + std::to_string(card.getCardNumber()) + " but the game was not in appropriate state");
			return false;
		}
		if(!canChangeColor(card) && chosenIndex != CardIndex::NULLINDEX) {
			log(LogSeverity::ERR, "Played card " + std::to_string(card.getCardNumber()) + "can't change card index but the chosen card index wasn't NULLINDEX");
			return false;
		}
		
		bool moveSuccess = movePlayedCardToPlayCardStack(player, card, wasCardJustDrawn);
		if(! moveSuccess) return false;

		updateColor(card, chosenIndex);
		updateDirection(card);
		markCardsFromDrawStackToDrawDueToPlusTwo(getAmountsOfCardsToDrawForNextPlayer(card));
		checkForMauIfNeeded();

		// if the option PASS_DRAW_TWO is active, the field is going to be cleared after the cards have been drawn
		std::vector<int> cardsToDrawDueToPlusTwo_backup = this->cardsToDrawDueToPlusTwo;
		std::vector<int> cardsToDrawDueToPlusTwo_filledWithNullcards = std::vector<int>(cardsToDrawDueToPlusTwo_backup.size(), 0);
		setNextPlayerOnTurnAndUpdateSkipState(card);		

		// send packet to players
		auto& senderPlayerPtr = lookupPlayerByUsername(player.getUsername());
		for(auto& p : allPlayers) {
			if(senderPlayerPtr == p) continue;

			std::vector<int>& cardsToDrawToSend = (allPlayers.isPlayerOnTurn(p)) ? cardsToDrawDueToPlusTwo_backup : cardsToDrawDueToPlusTwo_filledWithNullcards;
			OtherPlayerHasPlayedCard_STCPacket packet(player.getUsername(), card.getCardNumber(), static_cast<int>(chosenIndex), cardsToDrawToSend, wasCardJustDrawn);
			packetTransmitter->sendPacketToClient(packet, p.getWrappedParticipant());
		}

		callGameEndFunctIfGameHasEnded();

		return true;
	}

	bool ServerMauMauGame::movePlayedCardToPlayCardStack(Player& player, Card card, bool wasCardJustDrawn) {
		wasCardPlayed_thisTurn = true;

		if(wasCardJustDrawn) {
			if(drawCardStack.getLast() != card) {
				log(LogSeverity::ERR, "Player \"" + player.getUsername() + " erroneously pretended to have just drawn the played card");
				return false;
			}

			drawCardStack.removeLast();
			playCardStack.addFromPlain(card);
			tryRebalanceCardStacks();
			wasCardDrawn_thisTurn = true;
		} else {
			if(!player.containsHandCard(card)) {
				log(LogSeverity::ERR, "Player \"" + player.getUsername() + " erroneously pretended to haven't just drawn the played card or he tried to play a card which isn't owned by him.");
				return false;
			}

			player.removeHandCard(card);
			playCardStack.addFromPlain(card);
		} 

		return true;
	}
	void ServerMauMauGame::updateColor(Card playedCard, CardIndex chosenCardIndex) {
		if(canChangeColor(playedCard)) {
			this->indexForNextCard = chosenCardIndex;
		} else {
			this->indexForNextCard = playedCard.getCardIndex();
		}
	}
	void ServerMauMauGame::updateDirection(Card playedCard) {
		if(canChangeDirection(playedCard)) {
			if(direction == Direction::CW) direction = Direction::CCW;
			else direction = Direction::CW;
		}
	}
	std::vector<int> ServerMauMauGame::popCardsFromDrawStack(int cardAmount) {
		std::vector<int> removedCards;
		for(int i = 0; i < cardAmount; i++) {
			/*
			 * We intentionally remove the first card from draw card stack, since we send the last
			 * card on draw card stack as next card on draw stack to the client (if he has to draw a card).
			 * If this card is removed (by this method) from the card stack in the meantime (for example 
			 * due to a mau punishment event), we get an inconsistent data model.
			 */

			Card removed = drawCardStack.removeFirst();
			tryRebalanceCardStacks();

			removedCards.push_back(removed.getCardNumber());
		}
		return removedCards;
	}

	void ServerMauMauGame::markCardsFromDrawStackToDrawDueToPlusTwo(int cardAmount) {
		std::vector<int> cardsToDrawForNextPlayer = popCardsFromDrawStack(cardAmount);
		this->cardsToDrawDueToPlusTwo.insert(cardsToDrawDueToPlusTwo.begin(), cardsToDrawForNextPlayer.begin(), cardsToDrawForNextPlayer.end());
	}

	bool ServerMauMauGame::drawCardAndSetNextPlayerOnTurn(Player& player) {
		if(!canDraw(player)) return false;

		wasCardDrawn_thisTurn = true;

		Card removed = drawCardStack.removeLast();
		player.addHandCard(removed);
		tryRebalanceCardStacks();

		if(wasMauedCorrectly_thisTurn) {
			sendMauPunishmentPacket(player, MauPunishmentCause::DRAWED_EVEN_THOUGH_MAUED);
		}

		setNextPlayerOnTurn();

		// send packet to other players
		OtherPlayerHasDrawnCards_STCPacket otherPlayerHasDrawnPacket(player.getUsername());
		for(auto& p : allPlayers) {
			if(p == player) continue;
			packetTransmitter->sendPacketToClient(otherPlayerHasDrawnPacket, p.getWrappedParticipant());
		}

		return true;
	}

	bool ServerMauMauGame::pass(Player& player) {
		if(! canPass(player)) return false;
		setNextPlayerOnTurn();

		OtherPlayerHasPassed_STCPacket packet(player.getUsername());
		for(auto& p : allPlayers) {
			if(p == player) continue;
			packetTransmitter->sendPacketToClient(packet, p.getWrappedParticipant());
		}

		return true;
	}

	void ServerMauMauGame::setNextPlayerOnTurnAndUpdateSkipState(Card playedCard) {
		if(roomOptions.getOption(Options::PASS_SKIP)) {
			setNextPlayerOnTurn();
			if(canSkipPlayer(playedCard)) {
				isInSkipState_field = true;
			}
		} else {
			if(canSkipPlayer(playedCard)) setNextButOnePlayerOnTurn();
			else setNextPlayerOnTurn();
		}
	}

	void ServerMauMauGame::setNextPlayerOnTurn() {
		auto& nextPlayer = allPlayers.getNextPlayerOnTurn(direction);
		setPlayerOnTurn(nextPlayer);
	}

	void ServerMauMauGame::setNextButOnePlayerOnTurn() {
		auto& nextPlayer = allPlayers.getNextPlayerOnTurn(direction);
		auto& nextButOnePlayer = allPlayers.getNextPlayerOnTurn(direction, nextPlayer);

		setPlayerOnTurn(nextButOnePlayer);
	}

	void ServerMauMauGame::setPlayerOnTurn(Player& player) {
		amountOfDrawedCardsDueToPlusTwoLastTurn = 0;

		auto& lastPlayerOnTurn = allPlayers.getPlayerOnTurn();

		if(isInDrawTwoState() && !wasCardPlayed_thisTurn) {
			// if there was a card played, the +2 was passed on to the next player
			amountOfDrawedCardsDueToPlusTwoLastTurn = cardsToDrawDueToPlusTwo.size();
			lastPlayerOnTurn.addHandCards(cardsToDrawDueToPlusTwo);
			cardsToDrawDueToPlusTwo.clear();
		}
		if(isInDrawTwoState() && !roomOptions.getOption(Options::PASS_DRAW_TWO)) {
			// the cards are actually drawn later at the end of the method
			// If the PASS_DRAW_TWO option is NOT active, wasCardPlayed_thisTurn cannot be false if there are cards to draw,
			// since the player, which is on turn at the moment, must have played the +2-card.
			amountOfDrawedCardsDueToPlusTwoLastTurn = cardsToDrawDueToPlusTwo.size();
		}

		// reset fields
		wasCardDrawn_lastTurn = wasCardDrawn_thisTurn;
		wasCardDrawn_thisTurn = false;
		wasCardPlayed_lastTurn = wasCardPlayed_thisTurn;
		wasCardPlayed_thisTurn = false;
		wasMauedCorrectly_thisTurn = false;
		isInSkipState_field = false;

		startTurnAbortTimer();

		lastPlayerOnTurn.onEndTurn();
		allPlayers.setPlayerOnTurn(player);
		player.onStartTurn();

		// if there are still cards to draw and the option PASS_DRAW_TWO is not set, we draw the punishment cards
		if(isInDrawTwoState() && !roomOptions.getOption(Options::PASS_DRAW_TWO)) {
			player.addHandCards(cardsToDrawDueToPlusTwo);
			cardsToDrawDueToPlusTwo.clear();
		}

		Card nextOnDrawStackToSend = drawCardStack.getLast();
		LocalPlayerIsOnTurn_STCPacket packet(nextOnDrawStackToSend.getCardNumber());
		packetTransmitter->sendPacketToClient(packet, player.getWrappedParticipant());
	}
	void ServerMauMauGame::checkForMauIfNeeded() {
		bool isMauDisabledByOptions = !roomOptions.getOption(Options::HAVE_TO_MAU);
		if(isMauDisabledByOptions) return;

		auto& playerOnTurn = allPlayers.getPlayerOnTurn();
		if(wasCardDrawn_thisTurn && wasMauedCorrectly_thisTurn) {
			sendMauPunishmentPacket(playerOnTurn, MauPunishmentCause::DRAWED_EVEN_THOUGH_MAUED);
			return;
		}

		if(wasCardDrawn_thisTurn && wasCardPlayed_thisTurn) {
			// we don't check for mau if the player has drawed and played a card in the same turn, but only if he doesn't have maued
			return;
		} else if(wasCardDrawn_thisTurn) {
			log(LogSeverity::WARNING, "Logic error. Can't check for mau if the player has drawed but not played a card.");
		}

		bool hasJustPlayedLastButOneCard = playerOnTurn.getHandCards().getSize() == 1;
		bool wasntMaued = !wasMauedCorrectly_thisTurn;
		if(hasJustPlayedLastButOneCard && wasntMaued) {
			sendMauPunishmentPacket(playerOnTurn, MauPunishmentCause::NO_MAU_RECEIVED);
		}
	}
	void ServerMauMauGame::sendMauPunishmentPacket(Player& responsiblePlayer, MauPunishmentCause cause) {
		std::string responsiblePlayerUsername = responsiblePlayer.getUsername();
		std::vector<int> cardsToDraw = popCardsFromDrawStack(CARDS_TO_DRAW_MAU_PUNISHMENT);
		responsiblePlayer.addHandCards(cardsToDraw);

		std::vector<int> cardsToDrawAsNullCards = cardsToDraw;
		std::fill(cardsToDrawAsNullCards.begin(), cardsToDrawAsNullCards.end(), Card::NULLCARD.getCardNumber());

		for(auto& p : allPlayers) {
			if(p == responsiblePlayer) {
				MauPunishment_STCPacket packet(responsiblePlayerUsername, cardsToDraw, cause);
				packetTransmitter->sendPacketToClient(packet, p.getWrappedParticipant());
			} else {
				MauPunishment_STCPacket packet(responsiblePlayerUsername, cardsToDrawAsNullCards, cause);
				packetTransmitter->sendPacketToClient(packet, p.getWrappedParticipant());
			}
		}
	}

	void ServerMauMauGame::sendSuccessfulMauPacket(Player& responsiblePlayer) {
		std::string responsiblePlayerUsername = responsiblePlayer.getUsername();

		PlayerHasMauedSuccessfully_STCPacket packet(responsiblePlayerUsername);
		std::vector<std::shared_ptr<ParticipantOnServer>> playersAsParticipants = Player::getVectorWithWrappedParticipants(allPlayers.getAllPlayers());
		packetTransmitter->sendPacketToClients(packet, playersAsParticipants);
	}

	void ServerMauMauGame::startTurnAbortTimer() {
		uint64_t currentTurnAbortId = ++startTurnAbortIdCounter;

		int delay = MAX_TURN_DURATION + getTimeToSetNextPlayerOnTurn(wasCardPlayedLastTurn(), wasCardDrawnLastTurn(), getAmountOfDrawedCardsDueToPlusTwoLastTurn());
		threadUtils_invokeIn(delay, this, [this, currentTurnAbortId]() {
			if(startTurnAbortIdCounter == currentTurnAbortId) {
				// if startTurnAbortTimer() was not called in meantime
				abortTurn();
			}
		});
	}
	void ServerMauMauGame::abortTurn() {
		// if a player has drawn a card, but not finished his turn, the card is put back on the draw card stack
		// if we wouldn't shuffle the draw card stack, he would draw the same card again
		drawCardStack.shuffle();

		auto& playerOnTurn = allPlayers.getPlayerOnTurn();
		std::vector<int> cardsToDraw = popCardsFromDrawStack(CARDS_TO_DRAW_ON_TIME_EXPIRED);
		playerOnTurn.addHandCards(cardsToDraw);

		std::vector<int> cardsToDrawAsNullCards = cardsToDraw;
		std::fill(cardsToDrawAsNullCards.begin(), cardsToDrawAsNullCards.end(), Card::NULLCARD.getCardNumber());

		int amountOfCardsToDrawBefore = this->cardsToDrawDueToPlusTwo.size();
		
		for(auto& p : allPlayers) {
			if(p == playerOnTurn) {
				TurnWasAborted_STCPacket packet(cardsToDraw, amountOfCardsToDrawBefore);
				packetTransmitter->sendPacketToClient(packet, p.getWrappedParticipant());
			} else {
				TurnWasAborted_STCPacket packet(cardsToDrawAsNullCards, amountOfCardsToDrawBefore);
				packetTransmitter->sendPacketToClient(packet, p.getWrappedParticipant());
			}
		}

		setNextPlayerOnTurn();
	}
	bool ServerMauMauGame::canPlay(Player& player, Card card) const {
		if(allPlayers.getPlayerOnTurn() != player) return false;
		if(isInSkipState() && card.getValue() != SKIP_VALUE) return false;
		if(isInDrawTwoState() && card.getValue() != DRAW_2_VALUE) return false;

		Card lastCardOnPlayStack = playCardStack.getLast();
		if(card.getValue() == CHANGE_COLOR_VALUE && roomOptions.getOption(Options::CHOOSE_COLOR_ON_JACK)) {
			if(! roomOptions.getOption(Options::CAN_PUT_JACK_ON_JACK) && lastCardOnPlayStack.getValue() == CHANGE_COLOR_VALUE) return false;
			if(roomOptions.getOption(Options::CAN_PUT_JACK_ON_EVERY_COLOR)) return true;
		}

		return card.getCardIndex() == indexForNextCard || card.getValue() == lastCardOnPlayStack.getValue();
	}
	bool ServerMauMauGame::canDraw(Player& player) const {
		if(allPlayers.getPlayerOnTurn() != player) return false;
		if(canPass(player)) return false;
		return true;
	}
	bool ServerMauMauGame::canChangeColor(Card playedCard) const {
		return playedCard.getValue() == CHANGE_COLOR_VALUE && roomOptions.getOption(Options::CHOOSE_COLOR_ON_JACK);
	}
	int ServerMauMauGame::getAmountsOfCardsToDrawForNextPlayer(Card playedCard) const {
		if(!roomOptions.getOption(Options::DRAW_TWO_ON_SEVEN)) return false;
		return (playedCard.getValue() == DRAW_2_VALUE) ? 2 : 0;
	}
	bool ServerMauMauGame::canSkipPlayer(Card playedCard) const {
		return playedCard.getValue() == SKIP_VALUE && roomOptions.getOption(Options::SKIP_ON_EIGHT);
	}
	bool ServerMauMauGame::canChangeDirection(Card playedCard) const {
		return playedCard.getValue() == CHANGE_DIRECTION_VALUE && roomOptions.getOption(Options::DIRECTION_CHANGE_ON_NINE);
	}
	bool ServerMauMauGame::canMau(Player& player) const {
		return checkIfOnTurn(player) && player.getHandCards().getSize() == 2;
	}
	bool ServerMauMauGame::canPass(Player& player) const {
		return checkIfOnTurn(player) && (isInSkipState() || isInDrawTwoState());
	}
	bool ServerMauMauGame::isInSkipState() const {
		return isInSkipState_field;
	}
	bool ServerMauMauGame::isInDrawTwoState() const {
		return cardsToDrawDueToPlusTwo.size() > 0;
	}
	bool ServerMauMauGame::wasCardDrawnLastTurn() const {
		return wasCardDrawn_lastTurn;
	}
	bool ServerMauMauGame::wasCardPlayedLastTurn() const {
		return wasCardPlayed_lastTurn;
	}
	bool ServerMauMauGame::wasCardDrawnAndPlayedLastTurn() const {
		return wasCardDrawnLastTurn() && wasCardPlayedLastTurn();
	}
	int ServerMauMauGame::getAmountOfDrawedCardsDueToPlusTwoLastTurn() const {
		return amountOfDrawedCardsDueToPlusTwoLastTurn;
	}
	bool ServerMauMauGame::checkIfPlayerByParticipant(const ParticipantOnServer& participant) {
		for(auto& p : allPlayers) {
			if(p.getWrappedParticipant() == participant) return true;
		}
		return false;
	}
	bool ServerMauMauGame::checkIfPlayerByUsername(std::string username) {
		for(auto& p : allPlayers) {
			if(p.getUsername() == username) return true;
		}
		return false;
	}
	bool ServerMauMauGame::checkIfOnTurn(Player& p) const {
		return allPlayers.getPlayerOnTurn() == p;
	}
	Player& ServerMauMauGame::getPlayerOnTurn() {
		return allPlayers.getPlayerOnTurn();
	}
	Player& ServerMauMauGame::lookupPlayerByParticipant(const ParticipantOnServer& participant) {
		for(auto& p : allPlayers) {
			if(p.getWrappedParticipant() == participant) return p;
		}

		throw PlayerNotFoundException("This participant is not a player.");
	}
	Player& ServerMauMauGame::lookupPlayerByUsername(std::string username) {
		for(auto& p : allPlayers) {
			if(p.getUsername() == username) return p;
		}

		throw PlayerNotFoundException("\"" + username + "\" is not a player.");
	}
	void ServerMauMauGame::removePlayer(Player& player) {
		if(allPlayers.isPlayerOnTurn(player)) {
			setNextPlayerOnTurn();
		}

		allPlayers.removePlayer(player);
	}
	const RoomOptions& ServerMauMauGame::getOptions() const {
		return roomOptions;
	}
	std::size_t ServerMauMauGame::getAmountOfParticipants() const {
		return allPlayers.getAmountOfPlayers();
	}
	const CardStack& ServerMauMauGame::getPlayCardStack() const {
		return playCardStack;
	}
	const CardStack& ServerMauMauGame::getDrawCardStack() const {
		return drawCardStack;
	}
	CardIndex ServerMauMauGame::getCardIndexToPlay() const {
		return this->indexForNextCard;
	}
	void ServerMauMauGame::tryRebalanceCardStacks() {
		while(drawCardStack.getSize() <= MIN_DRAW_CARD_STACK_SIZE && playCardStack.getSize() > 1) {
			drawCardStack.addFromPlainAtPosition(0, playCardStack.remove(0));
		}
		if(drawCardStack.getSize() <= MIN_DRAW_CARD_STACK_SIZE) {
			addCardDeckToDrawStack();
		}
	}
	void ServerMauMauGame::addCardDeckToDrawStack() {
		CardStack cardsToAdd = {};
		cardsToAdd.fillWithCardDeckAndShuffle();
		cardsIngameSum += cardsToAdd.getSize();

		for(auto& c : cardsToAdd) {
			drawCardStack.addFromPlainAtPosition(0, c);
		}
		if(cardsIngameSum > MAX_CARDS_INGAME) {
			// after onGameEnd is called, this object is destroyed
			threadUtils_invokeIn(0, this, [this]() {
				GameWasAborted_STCPacket packet;
				packetTransmitter->sendPacketToClients(packet, Player::getVectorWithWrappedParticipants(allPlayers.getAllPlayers()));
				gameEndHandler.onGameEnd();
			});
		}
	}
	void ServerMauMauGame::callGameEndFunctIfGameHasEnded() {
		if(hasPlayerWon()) {
			gameEndHandler.onGameEnd();
		}
	}
	bool ServerMauMauGame::hasPlayerWon() {
		for(auto& p : allPlayers) {
			if(p.getHandCards().isEmpty()) {
				return true;
			}
		}
		return false;
	}
	std::optional<OperationSuccessful_STCAnswerPacket> ServerMauMauGame::listener_onPlayCard(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant) {
		if(! checkIfPlayerByParticipant(*participant)) return std::nullopt;
		auto& casted = dynamic_cast<PlayCardRequest_CTSPacket&>(p);

		auto& player = lookupPlayerByParticipant(*participant);
		bool wasSuccessful = playCardAndSetNextPlayerOnTurn(player, Card(casted.getCardNumber()), casted.wasJustDrawn(), static_cast<CardIndex>(casted.getNewCardIndex()));
		return OperationSuccessful_STCAnswerPacket(wasSuccessful);
	}
	std::optional<OperationSuccessful_STCAnswerPacket> ServerMauMauGame::listener_onDrawCard(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant) {
		if(!checkIfPlayerByParticipant(*participant)) return std::nullopt;
		auto& casted = dynamic_cast<DrawCardRequest_CTSPacket&>(p);

		auto& player = lookupPlayerByParticipant(*participant);
		bool wasSuccessful = drawCardAndSetNextPlayerOnTurn(player);
		return OperationSuccessful_STCAnswerPacket(wasSuccessful);
	}
	std::optional<OperationSuccessful_STCAnswerPacket> ServerMauMauGame::listener_onMau(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant) {
		if(!checkIfPlayerByParticipant(*participant)) return std::nullopt;
		auto& casted = dynamic_cast<MauRequest_CTSPacket&>(p);

		auto& player = lookupPlayerByParticipant(*participant);
		mau(player);
		return OperationSuccessful_STCAnswerPacket(true);
	}
	std::optional<OperationSuccessful_STCAnswerPacket> ServerMauMauGame::listener_onPass(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant) {
		if(!checkIfPlayerByParticipant(*participant)) return std::nullopt;
		auto& casted = dynamic_cast<PassRequest_CTSPacket&>(p);
		auto& player = lookupPlayerByParticipant(*participant);
		bool wasSuccessful = pass(player);
		return OperationSuccessful_STCAnswerPacket(wasSuccessful);


	}
}