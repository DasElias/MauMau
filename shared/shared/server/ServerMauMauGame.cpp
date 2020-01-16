#include "ServerMauMauGame.h"
#include "AiPlayer.h"

#include "../packet/cts/DrawCardRequest_CTSPacket.h"
#include "../packet/cts/PlayCardRequest_CTSPacket.h"

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

namespace card {
	ServerMauMauGame::ServerMauMauGame(std::shared_ptr<STCPacketTransmitter> packetTransmitter, std::vector<std::shared_ptr<ParticipantOnServer>> participants) :
			packetTransmitter(packetTransmitter),
			drawCardStack(),
			playCardStack(),
			handler_onPlayCard(std::bind(&ServerMauMauGame::listener_onPlayCard, this, std::placeholders::_1, std::placeholders::_2)),
			handler_onDrawCard(std::bind(&ServerMauMauGame::listener_onDrawCard, this, std::placeholders::_1, std::placeholders::_2)){

		drawCardStack.fillWithCardDeckAndShuffle();

		Card firstCardOnPlayStack;

		// init play card stack
		for(std::size_t i = 0; i < drawCardStack.getSize(); i++) {
			auto card = drawCardStack.get(i);
			if(!canChangeColor(card)) {
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
	}

	ServerMauMauGame::~ServerMauMauGame() {
		packetTransmitter->removeListenerForClientPkt(PlayCardRequest_CTSPacket::PACKET_ID, handler_onPlayCard);
		packetTransmitter->removeListenerForClientPkt(DrawCardRequest_CTSPacket::PACKET_ID, handler_onDrawCard);
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
	void ServerMauMauGame::setInitialPlayerOnTurn() {
		auto newPlayerOnTurn = getRandomPlayer();
		this->playerOnTurn = newPlayerOnTurn;

		int timeUntilCardsDistributed = getDurationUntilInitialCardsAreDistributed(this->players.size(), AMOUNT_OF_HAND_CARDS);
		threadUtils_invokeIn(timeUntilCardsDistributed, [this]() {
			playerOnTurn->onStartTurn();
		});
	}
	void ServerMauMauGame::setPlayerOnTurn(std::shared_ptr<Player> player) {
		this->playerOnTurn->onEndTurn();
		this->playerOnTurn = player;
		this->playerOnTurn->onStartTurn();

		Card nextOnDrawStackToSend = drawCardStack.getLast();
		LocalPlayerIsOnTurn_STCPacket packet(nextOnDrawStackToSend.getCardNumber());
		packetTransmitter->sendPacketToClient(packet, player->getWrappedParticipant());
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
	bool ServerMauMauGame::drawCardAndSetNextPlayerOnTurn(Player& player) {
		if(! canDraw(player)) return false;

		Card removed = drawCardStack.removeLast();
		player.addHandCard(removed);
		tryRebalanceCardStacks();

		setNextPlayerOnTurn();

		// send packet to other players
		OtherPlayerHasDrawnCards_STCPacket otherPlayerHasDrawnPacket(player.getUsername());
		for(auto& p : this->players) {
			if(*p == player) continue;
			packetTransmitter->sendPacketToClient(otherPlayerHasDrawnPacket, p->getWrappedParticipant());
		}

		return true;
	}
	bool ServerMauMauGame::playCardAndSetNextPlayerOnTurn(Player& player, Card card, CardIndex chosenIndex) {
		if(! canPlay(player, card)) return false;
		if(! canChangeColor(card) && chosenIndex != CardIndex::NULLINDEX) return false;

		bool wasCardJustDrawn = false;
		if(! player.containsHandCard(card) && drawCardStack.getLast() == card) {
			// player hasn't the drawn card in his hand cards, but it's the last on the drawCardStack, so he must have just drawn it

			drawCardStack.removeLast();
			playCardStack.addFromPlain(card);
			wasCardJustDrawn = true;
		} else if(player.containsHandCard(card)) {
			// player wants to play a card in his hand card

			player.removeHandCard(card);
			playCardStack.addFromPlain(card);
		} else {
			// player tries to play a card which isn't owned by him

			return false;	
		}
		
		// compute cards to draw for next player
		// it's important to compute those before the next player is set on turn,
		// since afterwards we've send already the last card on draw stack to the new
		// player on turn
		std::vector<int> cardsToDrawForNextPlayer;
		for(int i = 0; i < getAmountsOfCardsToDrawForNextPlayer(card); i++) {
			Card removed = drawCardStack.removeLast();
			tryRebalanceCardStacks();
			
			cardsToDrawForNextPlayer.push_back(removed.getCardNumber());
		}
		
		// set next player on turn
		if(canSkipPlayer(card)) setNextButOnePlayerOnTurn();
		else setNextPlayerOnTurn();

		// actually draw cardsToDrawForNextPlayer
		for(auto& cardNumber : cardsToDrawForNextPlayer) {
			playerOnTurn->addHandCard(Card(cardNumber));
		}
		
		// change color
		if(canChangeColor(card)) {
			log(LogSeverity::INFO, "color changed");
			this->indexForNextCard = chosenIndex;
		} else {
			this->indexForNextCard = card.getCardIndex();
		}

		// send packet to all other players
		auto senderPlayerPtr = lookupPlayerByUsername(player.getUsername());
		for(auto& p : this->players) {
			if(senderPlayerPtr == p) continue;

			std::vector<int> cardsToDrawToSend = (playerOnTurn == p) ? cardsToDrawForNextPlayer : std::vector<int>(cardsToDrawForNextPlayer.size(), 0);

			OtherPlayerHasPlayedCard_STCPacket packet(player.getUsername(), card.getCardNumber(), static_cast<int>(chosenIndex), cardsToDrawToSend, wasCardJustDrawn);
			packetTransmitter->sendPacketToClient(packet, p->getWrappedParticipant());
		}

		return true;
	}
	void ServerMauMauGame::tryRebalanceCardStacks() {
		while(drawCardStack.getSize() <= 3 && playCardStack.getSize() >= 1) {
			drawCardStack.addFromPlain(playCardStack.remove(0));
		}
	}
	std::optional<OperationSuccessful_STCAnswerPacket> ServerMauMauGame::listener_onPlayCard(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant) {
		if(! checkIfPlayerByParticipant(participant)) return std::nullopt;
		auto& casted = dynamic_cast<PlayCardRequest_CTSPacket&>(p);

		auto player = lookupPlayerByParticipant(participant);
		bool wasSuccessful = playCardAndSetNextPlayerOnTurn(*player, {casted.getCardNumber()}, static_cast<CardIndex>(casted.getNewCardIndex()));
		return OperationSuccessful_STCAnswerPacket(wasSuccessful);
	}
	std::optional<OperationSuccessful_STCAnswerPacket> ServerMauMauGame::listener_onDrawCard(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant) {
		if(!checkIfPlayerByParticipant(participant)) return std::nullopt;
		auto& casted = dynamic_cast<DrawCardRequest_CTSPacket&>(p);

		auto player = lookupPlayerByParticipant(participant);
		bool wasSuccessful = drawCardAndSetNextPlayerOnTurn(*player);
		return OperationSuccessful_STCAnswerPacket(wasSuccessful);
	}
}