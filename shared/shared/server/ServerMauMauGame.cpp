#include "ServerMauMauGame.h"
#include "AiPlayer.h"

#include "../packet/cts/MauRequest_CTSPacket.h"
#include "../packet/cts/DrawCardRequest_CTSPacket.h"
#include "../packet/cts/PlayCardRequest_CTSPacket.h"

#include "../packet/stc/OtherPlayerHasMauedSuccessfully_STCPacket.h"
#include "../packet/stc/MauPunishment_STCPacket.h"
#include "../packet/stc/TurnWasAborted_STCPacket.h"
#include "../packet/stc/LocalPlayerIsOnTurn_STCPacket.h"
#include "../packet/stc/OtherPlayerHasDrawnCards_STCPacket.h"
#include "../packet/stc/OtherPlayerHasPlayedCard_STCPacket.h"
#include "../packet/stc/GameHasBeenStarted_STCPacket.h"
#include "../model/PlayerNotFoundException.h"
#include "../model/MauMauCardValueMeanings.h"
#include "../utils/Logger.h"
#include <iostream>

#include "../utils/MathUtils.h"
#include "../utils/ThreadUtils.h"
#include "../model/CardAnimationDuration.h"
#include "../model/TimeToSetNextPlayerOnTurnDuration.h"
#include "../model/MaxTurnDuration.h"

namespace card {
	uint64_t ServerMauMauGame::startTurnAbortIdCounter = 0;

	ServerMauMauGame::ServerMauMauGame(std::shared_ptr<STCPacketTransmitter> packetTransmitter, ServerGameEndHandler& gameEndHandler, std::vector<std::shared_ptr<ParticipantOnServer>> participants) :
			packetTransmitter(packetTransmitter),
			gameEndHandler(gameEndHandler),
			drawCardStack(),
			playCardStack(),
			handler_onPlayCard(std::bind(&ServerMauMauGame::listener_onPlayCard, this, std::placeholders::_1, std::placeholders::_2)),
			handler_onDrawCard(std::bind(&ServerMauMauGame::listener_onDrawCard, this, std::placeholders::_1, std::placeholders::_2)),
			handler_onMau(std::bind(&ServerMauMauGame::listener_onMau, this, std::placeholders::_1, std::placeholders::_2)) {

		drawCardStack.fillWithCardDeckAndShuffle();

		Card firstCardOnPlayStack;

		// init play card stack
		for(std::size_t i = 0; i < drawCardStack.getSize(); i++) {
			auto card = drawCardStack.get(i);
			if(! canChangeColor(card) && getAmountsOfCardsToDrawForNextPlayer(card) == 0 && !canSkipPlayer(card)) {
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
			this->players.push_back(constructedPlayer);

			for(int i = 0; i < AMOUNT_OF_HAND_CARDS; i++) {
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
	}
	
	ServerMauMauGame::~ServerMauMauGame() {
		packetTransmitter->removeListenerForClientPkt(PlayCardRequest_CTSPacket::PACKET_ID, handler_onPlayCard);
		packetTransmitter->removeListenerForClientPkt(DrawCardRequest_CTSPacket::PACKET_ID, handler_onDrawCard);
		packetTransmitter->removeListenerForClientPkt(MauRequest_CTSPacket::PACKET_ID, handler_onMau);


		// turn of current player doesn't expire
		startTurnAbortIdCounter++;
	}

	void ServerMauMauGame::setInitialPlayerOnTurn() {
		auto newPlayerOnTurn = getRandomPlayer();
		this->playerOnTurn = newPlayerOnTurn;

		int timeUntilCardsDistributed = getDurationUntilInitialCardsAreDistributed(this->players.size(), AMOUNT_OF_HAND_CARDS);
		threadUtils_invokeIn(timeUntilCardsDistributed, [this]() {
			playerOnTurn->onStartTurn();
			startTurnAbortTimer();
		});


	}

	void ServerMauMauGame::mau(Player& player) {
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

	bool ServerMauMauGame::playCardAndSetNextPlayerOnTurn(Player& player, Card card, CardIndex chosenIndex) {
		if(!canPlay(player, card)) {
			log(LogSeverity::ERR, "Player " + player.getUsername() + " tried to play card " + std::to_string(card.getCardNumber()) + " but the game was not in appropriate state");
			return false;
		}
		if(!canChangeColor(card) && chosenIndex != CardIndex::NULLINDEX) {
			log(LogSeverity::ERR, "Played card " + std::to_string(card.getCardNumber()) + "can't change card index but the chosen card index wasn't NULLINDEX");
			return false;
		}

		bool wasCardJustDrawn = false;
		bool moveSuccess = movePlayedCardToPlayCardStack(player, card, wasCardJustDrawn);
		if(! moveSuccess) return false;

		// compute cards to draw for next player
		// it's important to compute those before the next player is set on turn,
		// since afterwards we've send already the last card on draw stack to the new
		// player on turn
		std::vector<int> cardsToDrawForNextPlayer = popCardsFromDrawStack(getAmountsOfCardsToDrawForNextPlayer(card));

		if(!hasPlayerWon()) {
			checkForMauIfNeeded();

			setNextOrNextButOneOnTurnLocal(card);

			// actually draw cardsToDrawForNextPlayer
			playerOnTurn->addHandCards(cardsToDrawForNextPlayer);

		}

		updateColor(card, chosenIndex);	

		// send packet to all other players
		auto senderPlayerPtr = lookupPlayerByUsername(player.getUsername());
		for(auto& p : this->players) {
			if(senderPlayerPtr == p) continue;

			std::vector<int> cardsToDrawToSend = (playerOnTurn == p) ? cardsToDrawForNextPlayer : std::vector<int>(cardsToDrawForNextPlayer.size(), 0);

			OtherPlayerHasPlayedCard_STCPacket packet(player.getUsername(), card.getCardNumber(), static_cast<int>(chosenIndex), cardsToDrawToSend, wasCardJustDrawn);
			packetTransmitter->sendPacketToClient(packet, p->getWrappedParticipant());
		}

		callGameEndFunctIfGameHasEnded();

		return true;
	}

	bool ServerMauMauGame::movePlayedCardToPlayCardStack(Player& player, Card card, bool& out_wasCardDrawnAndPlayed) {
		wasCardPlayed_thisTurn = true;

		if(!player.containsHandCard(card) && drawCardStack.getLast() == card) {
			// player hasn't the drawn card in his hand cards, but it's the last on the drawCardStack, so he must have just drawn it

			drawCardStack.removeLast();
			playCardStack.addFromPlain(card);
			wasCardDrawn_thisTurn = true;
			out_wasCardDrawnAndPlayed = true;
		} else if(player.containsHandCard(card)) {
			// player wants to play a card in his hand card

			player.removeHandCard(card);
			playCardStack.addFromPlain(card);
			out_wasCardDrawnAndPlayed = false;
		} else {
			// player tries to play a card which isn't owned by him

			return false;
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
	std::vector<int> ServerMauMauGame::popCardsFromDrawStack(int cardAmount) {
		std::vector<int> removedCards;
		for(int i = 0; i < cardAmount; i++) {
			Card removed = drawCardStack.removeLast();
			tryRebalanceCardStacks();

			removedCards.push_back(removed.getCardNumber());
		}
		return removedCards;
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
		for(auto& p : this->players) {
			if(*p == player) continue;
			packetTransmitter->sendPacketToClient(otherPlayerHasDrawnPacket, p->getWrappedParticipant());
		}

		return true;
	}

	void ServerMauMauGame::setNextOrNextButOneOnTurnLocal(Card playedCard) {
		if(canSkipPlayer(playedCard)) setNextButOnePlayerOnTurn();
		else setNextPlayerOnTurn();
	}

	void ServerMauMauGame::setNextPlayerOnTurn() {
		auto playerOnTurnIter = std::find(players.begin(), players.end(), playerOnTurn);
		playerOnTurnIter++;

		if(playerOnTurnIter == players.end()) playerOnTurnIter = players.begin();
		setPlayerOnTurn(*playerOnTurnIter);
	}

	void ServerMauMauGame::setNextButOnePlayerOnTurn() {
		auto playerOnTurnIter = std::find(players.begin(), players.end(), playerOnTurn);

		for(int i = 0; i < 2; i++) {
			playerOnTurnIter++;
			if(playerOnTurnIter == players.end()) playerOnTurnIter = players.begin();
		}

		setPlayerOnTurn(*playerOnTurnIter);
	}

	void ServerMauMauGame::setPlayerOnTurn(std::shared_ptr<Player> player) {
		wasCardDrawn_lastTurn = wasCardDrawn_thisTurn;
		wasCardDrawn_thisTurn = false;

		wasCardPlayed_lastTurn = wasCardPlayed_thisTurn;
		wasCardPlayed_thisTurn = false;

		wasMauedCorrectly_thisTurn = false;

		startTurnAbortTimer();

		this->playerOnTurn->onEndTurn();
		this->playerOnTurn = player;
		this->playerOnTurn->onStartTurn();

		Card nextOnDrawStackToSend = drawCardStack.getLast();
		LocalPlayerIsOnTurn_STCPacket packet(nextOnDrawStackToSend.getCardNumber());
		packetTransmitter->sendPacketToClient(packet, player->getWrappedParticipant());
	}
	void ServerMauMauGame::checkForMauIfNeeded() {
		if(wasCardDrawn_thisTurn && wasCardPlayed_thisTurn) {
			// we don't check for mau if the player has drawed and played a card in the same turn
			return;
		} else if(wasCardDrawn_thisTurn) {
			throw std::runtime_error("Logic error. Can't check for mau if the player hasn't played a card-");
		}

		bool hasJustPlayedLastButOneCard = playerOnTurn->getHandCards().getSize() == 1;
		bool wasntMaued = !wasMauedCorrectly_thisTurn;
		if(hasJustPlayedLastButOneCard && wasntMaued) {
			sendMauPunishmentPacket(*playerOnTurn, MauPunishmentCause::NO_MAU_RECEIVED);
		}
	}
	void ServerMauMauGame::sendMauPunishmentPacket(Player& responsiblePlayer, MauPunishmentCause cause) {
		std::string responsiblePlayerUsername = responsiblePlayer.getUsername();
		std::vector<int> cardsToDraw = popCardsFromDrawStack(CARDS_TO_DRAW_MAU_PUNISHMENT);
		responsiblePlayer.addHandCards(cardsToDraw);

		std::vector<int> cardsToDrawAsNullCards = cardsToDraw;
		std::fill(cardsToDrawAsNullCards.begin(), cardsToDrawAsNullCards.end(), Card::NULLCARD.getCardNumber());

		for(auto& p : players) {
			if(*p == responsiblePlayer) {
				MauPunishment_STCPacket packet(responsiblePlayerUsername, cardsToDraw, cause);
				packetTransmitter->sendPacketToClient(packet, p->getWrappedParticipant());
			} else {
				MauPunishment_STCPacket packet(responsiblePlayerUsername, cardsToDrawAsNullCards, cause);
				packetTransmitter->sendPacketToClient(packet, p->getWrappedParticipant());
			}
		}
	}

	void ServerMauMauGame::sendSuccessfulMauPacket(Player& responsiblePlayer) {
		std::string responsiblePlayerUsername = responsiblePlayer.getUsername();
		for(auto& p : players) {
			if(! (*p == responsiblePlayer)) {
				OtherPlayerHasMauedSuccessfully_STCPacket packet(responsiblePlayerUsername);
				packetTransmitter->sendPacketToClient(packet, p->getWrappedParticipant());
			}
		}
	}

	void ServerMauMauGame::startTurnAbortTimer() {
		uint64_t currentTurnAbortId = ++startTurnAbortIdCounter;

		int delay = MAX_TURN_DURATION + getTimeToSetNextPlayerOnTurn(playCardStack.getSize(), playCardStack.getLast(), wasCardPlayedLastTurn(), wasCardDrawnLastTurn());
		threadUtils_invokeIn(delay, [this, currentTurnAbortId]() {

			if(startTurnAbortIdCounter == currentTurnAbortId) {
				// startTurnAbortTimer() was not called in meantime

				abortTurn();
			}
		});
	}
	void ServerMauMauGame::abortTurn() {
		// if a player has drawn a card, but not finished his turn, the card is put back on the draw card stack
		// if we wouldn't shuffle the draw card stack, he would draw the same card again
		drawCardStack.shuffle();

		std::vector<int> cardsToDraw = popCardsFromDrawStack(CARDS_TO_DRAW_ON_TIME_EXPIRED);
		playerOnTurn->addHandCards(cardsToDraw);

		std::vector<int> cardsToDrawAsNullCards = cardsToDraw;
		std::fill(cardsToDrawAsNullCards.begin(), cardsToDrawAsNullCards.end(), Card::NULLCARD.getCardNumber());
		
		for(auto& p : players) {
			if(p == playerOnTurn) {
				TurnWasAborted_STCPacket packet(cardsToDraw);
				packetTransmitter->sendPacketToClient(packet, p->getWrappedParticipant());
			} else {
				TurnWasAborted_STCPacket packet(cardsToDrawAsNullCards);
				packetTransmitter->sendPacketToClient(packet, p->getWrappedParticipant());
			}
		}

		setNextPlayerOnTurn();
	}
	bool ServerMauMauGame::canPlay(Player& player, Card card) const {
		if(playerOnTurn->getUsername() != player.getUsername()) return false;

		Card lastCardOnPlayStack = playCardStack.getLast();
		if(lastCardOnPlayStack.getValue() == CHANGE_COLOR_VALUE) {
			// exception rule
			// can't play jack onto another jack card
			if(card.getValue() == CHANGE_COLOR_VALUE) return false;
		} else if(lastCardOnPlayStack.getCardIndex() != indexForNextCard) {
			throw std::runtime_error("Inconsistent model_test state! The only situation when the index for the next card isn't equal to the index of the last card is when the last card played has been a JACK.");
		}

		return card.getCardIndex() == indexForNextCard || card.getValue() == lastCardOnPlayStack.getValue();
	}
	bool ServerMauMauGame::canDraw(Player& player) const {
		if(playerOnTurn->getUsername() != player.getUsername()) return false;

		return true;
	}
	bool ServerMauMauGame::canChangeColor(Card playedCard) const {
		return playedCard.getValue() == CHANGE_COLOR_VALUE;
	}
	int ServerMauMauGame::getAmountsOfCardsToDrawForNextPlayer(Card playedCard) const {
		return (playedCard.getValue() == DRAW_2_VALUE) ? 2 : 0;
	}
	bool ServerMauMauGame::canSkipPlayer(Card playedCard) const {
		return playedCard.getValue() == SKIP_VALUE;
	}
	bool ServerMauMauGame::canMau(Player& player) const {
		return checkIfOnTurn(player) && player.getHandCards().getSize() == 2;
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
	bool ServerMauMauGame::checkIfPlayerByParticipant(const std::shared_ptr<ParticipantOnServer>& participant) {
		for(auto& p : players) {
			if(p->getWrappedParticipant() == participant) return true;
		}
		return false;
	}
	bool ServerMauMauGame::checkIfPlayerByUsername(std::string username) {
		for(auto& p : players) {
			if(p->getUsername() == username) return true;
		}
		return false;
	}
	bool ServerMauMauGame::checkIfOnTurn(Player& p) const {
		return *playerOnTurn == p;
	}
	std::shared_ptr<Player> ServerMauMauGame::getPlayerOnTurn() {
		return playerOnTurn;
	}
	std::shared_ptr<Player> ServerMauMauGame::lookupPlayerByParticipant(const std::shared_ptr<ParticipantOnServer>& participant) {
		for(auto& p : players) {
			if(p->getWrappedParticipant() == participant) return p;
		}

		throw PlayerNotFoundException("This participant is not a player.");
	}
	std::shared_ptr<Player> ServerMauMauGame::lookupPlayerByUsername(std::string username) {
		for(auto& p : players) {
			if(p->getUsername() == username) return p;
		}

		throw PlayerNotFoundException("\"" + username + "\" is not a player.");
	}
	std::shared_ptr<Player> ServerMauMauGame::getRandomPlayer() {
		auto rand = randomInRange<std::size_t>(0, players.size() - 1);
		return players[rand];
	}
	void ServerMauMauGame::removePlayer(std::shared_ptr<Player> player) {
		if(player == playerOnTurn) {
			setNextPlayerOnTurn();
		}

		players.erase(std::remove(players.begin(), players.end(), player), players.end());

	}
	const CardStack& ServerMauMauGame::getPlayCardStack() const {
		return playCardStack;
	}
	const CardStack& ServerMauMauGame::getDrawCardStack() const {
		return drawCardStack;
	}
	void ServerMauMauGame::tryRebalanceCardStacks() {
		while(drawCardStack.getSize() <= 3 && playCardStack.getSize() >= 1) {
			drawCardStack.addFromPlain(playCardStack.remove(0));
		}
	}
	void ServerMauMauGame::callGameEndFunctIfGameHasEnded() {
		if(hasPlayerWon()) {
			gameEndHandler.onGameEnd();
		}
	}
	bool ServerMauMauGame::hasPlayerWon() {
		for(auto& p : players) {
			if(p->getHandCards().isEmpty()) {
				return true;
			}
		}
		return false;
	}
	std::optional<OperationSuccessful_STCAnswerPacket> ServerMauMauGame::listener_onPlayCard(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant) {
		if(! checkIfPlayerByParticipant(participant)) return std::nullopt;
		auto& casted = dynamic_cast<PlayCardRequest_CTSPacket&>(p);

		auto player = lookupPlayerByParticipant(participant);
		bool wasSuccessful = playCardAndSetNextPlayerOnTurn(*player, Card(casted.getCardNumber()), static_cast<CardIndex>(casted.getNewCardIndex()));
		return OperationSuccessful_STCAnswerPacket(wasSuccessful);
	}
	std::optional<OperationSuccessful_STCAnswerPacket> ServerMauMauGame::listener_onDrawCard(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant) {
		if(!checkIfPlayerByParticipant(participant)) return std::nullopt;
		auto& casted = dynamic_cast<DrawCardRequest_CTSPacket&>(p);

		auto player = lookupPlayerByParticipant(participant);
		bool wasSuccessful = drawCardAndSetNextPlayerOnTurn(*player);
		return OperationSuccessful_STCAnswerPacket(wasSuccessful);
	}
	std::optional<OperationSuccessful_STCAnswerPacket> ServerMauMauGame::listener_onMau(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant) {
		if(!checkIfPlayerByParticipant(participant)) return std::nullopt;
		auto& casted = dynamic_cast<MauRequest_CTSPacket&>(p);

		auto player = lookupPlayerByParticipant(participant);
		mau(*player);
		return OperationSuccessful_STCAnswerPacket(true);
	}
}