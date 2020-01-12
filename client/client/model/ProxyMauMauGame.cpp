#include "ProxyMauMauGame.h"
#include <shared/model/PlayerNotFoundException.h>

#include <shared/model/MauMauCardValueMeanings.h>

#include <shared/packet/cts/MauRequest_CTSPacket.h>
#include <shared/packet/cts/PlayCardRequest_CTSPacket.h>
#include <shared/packet/cts/DrawCardRequest_CTSPacket.h>

#include <shared/packet/stc/OtherPlayerHasDrawnCards_STCPacket.h>
#include <shared/packet/stc/OtherPlayerHasPlayedCard_STCPacket.h>
#include <shared/packet/stc/LocalPlayerIsOnTurn_STCPacket.h>

#include <iostream>
#include <shared/model/CardAnimationDuration.h>

namespace card {
	ProxyMauMauGame::ProxyMauMauGame(std::shared_ptr<CTSPacketTransmitter> packetTransmitter, std::vector<std::shared_ptr<ParticipantOnClient>> allParticipantsInclLocal, std::shared_ptr<ParticipantOnClient> localParticipant, std::string usernameOnTurn, std::vector<int> handCards, int startCard, int nextCardOnDrawStack, RoomOptions& roomOptions) :
			packetTransmitter(packetTransmitter),
			drawCardStack(std::make_unique<CardStack>()),
			playCardStack(std::make_unique<CardStack>(std::initializer_list<Card>({startCard}))),
			indexForNextCard(Card(startCard).getCardIndex()),
			roomOptions(roomOptions),
			handler_onOtherPlayerHasDrawnCard(std::bind(&ProxyMauMauGame::listener_onOtherPlayerHasDrawnCard, this, std::placeholders::_1)),
			handler_onOtherPlayerHasPlayedCard(std::bind(&ProxyMauMauGame::listener_onOtherPlayerHasPlayedCard, this, std::placeholders::_1)),
			handler_onLocalPlayerIsOnTurn(std::bind(&ProxyMauMauGame::listener_onLocalPlayerIsOnTurn, this, std::placeholders::_1)) {

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
		initStartCards(handCards);

		// initialize player on turn
		this->userOnTurn = lookupPlayer(usernameOnTurn);

		setLocalPlayerAtTheBeginOfPlayersVector();

		// set first draw card
		if(isLocalPlayerOnTurn() && nextCardOnDrawStack == Card::NULLCARD.getCardNumber()) throw std::runtime_error("Can't set a NULLCARD as next card on draw card stack");
		this->drawCardForNextPlayer = Card(nextCardOnDrawStack);

		// initialize listeners
		packetTransmitter->addListenerForServerPkt(OtherPlayerHasDrawnCards_STCPacket::PACKET_ID, handler_onOtherPlayerHasDrawnCard);
		packetTransmitter->addListenerForServerPkt(OtherPlayerHasPlayedCard_STCPacket::PACKET_ID, handler_onOtherPlayerHasPlayedCard);
		packetTransmitter->addListenerForServerPkt(LocalPlayerIsOnTurn_STCPacket::PACKET_ID, handler_onLocalPlayerIsOnTurn);
	}
	
	ProxyMauMauGame::~ProxyMauMauGame() {
		packetTransmitter->removeListenerForServerPkt(OtherPlayerHasDrawnCards_STCPacket::PACKET_ID, handler_onOtherPlayerHasDrawnCard);
		packetTransmitter->removeListenerForServerPkt(OtherPlayerHasPlayedCard_STCPacket::PACKET_ID, handler_onOtherPlayerHasPlayedCard);
		packetTransmitter->removeListenerForServerPkt(LocalPlayerIsOnTurn_STCPacket::PACKET_ID, handler_onLocalPlayerIsOnTurn);
	}

	void ProxyMauMauGame::initStartCards(const std::vector<int>& handCardNumbersOfLocalPlayer) {
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
		if(! isLocalPlayerOnTurn() || localPlayer->hasPlayedCard() || !areAllPreviousCardTransactionsCompleted()) return false;

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
		if(! isLocalPlayerOnTurn() || !areAllPreviousCardTransactionsCompleted() || isWaitingForColorChoose() || drawCardForNextPlayer == Card::NULLCARD) return false;

		// has already drawn a card
		if(localPlayer->hasStartedToDrawCard()) return false;

		return true;		
	}

	bool ProxyMauMauGame::canChangeColor(Card playedCard) const {
		return roomOptions.chooseCardIndexOnJack() && playedCard.getValue() == CHANGE_COLOR_VALUE;
	}

	bool ProxyMauMauGame::canMau() const {
		// player has already finished game, no more mau needed
		if(localPlayer->getCardStack().isEmptyAndNoPendingTransactions()) return false;

		// display warning to the player
		if(localPlayer->getCardStack().getSizeInclPendingTransactions() >= 3) return false;
		
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
		return localPlayer->isWaitingForColorPick();
	}

	std::size_t ProxyMauMauGame::getAmountsOfCardsToDrawForNextPlayer(Card playedCard) const {
		if(! roomOptions.drawTwoCardsOnSeven()) return false;
		return (playedCard.getValue() == DRAW_2_VALUE) ? 2 : 0;
	}

	void ProxyMauMauGame::drawCard() {
		if(! canDraw()) throw std::runtime_error("Can't draw card in the current situation!");
		if(drawCardForNextPlayer == Card::NULLCARD) throw std::runtime_error("Can't draw a NULLCARD");

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
		localPlayer->playCardFromHandCards(card, playCardStack, canChangeColor);

		if(! canChangeColor) {
			sendPlayCardPacket();
		}

		// else packet will be send in chooseColor()
	}

	void ProxyMauMauGame::playDrawnCard() {
		if(!canPlayDrawnCard()) throw std::runtime_error("Can't play drawn card!");
		
		Card drawnCard = *localPlayer->getDrawnCard();
		bool canChangeColor = this->canChangeColor(drawnCard);
		localPlayer->playCardFromTempCardStackLocal(playCardStack, canChangeColor);

		if(!canChangeColor) {
			sendPlayCardPacket();
		}

		// else packet will be send in chooseColor()
	}


	void ProxyMauMauGame::chooseColor(CardIndex color) {
		if(! isWaitingForColorChoose()) throw std::runtime_error("Can't choose color!");
		localPlayer->setColorWasPicked();

		this->indexForNextCard = color;
		sendPlayCardPacket(color);
	}

	void ProxyMauMauGame::sendPlayCardPacket(CardIndex newCardIndex) {
		auto playedCard = *localPlayer->getPlayedCard();
		PlayCardRequest_CTSPacket p(playedCard.getCardNumber(), static_cast<int>(newCardIndex));
		packetTransmitter->sendPacketToServer(p);

		setNextOrNextButOneOnTurnLocal(playedCard);

		std::size_t cardsToDrawForNextPlayer = getAmountsOfCardsToDrawForNextPlayer(playedCard);
		playerHasToDrawCards(this->userOnTurn, cardsToDrawForNextPlayer);

		this->drawCardForNextPlayer = Card::NULLCARD;
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
	void ProxyMauMauGame::playCardAndSetNextPlayerOnTurnLocal(std::string username, Card card, CardIndex newCardIndex, std::vector<Card> cardsToDraw, bool wasDrawedJustBefore) {
		std::shared_ptr<ProxyPlayer> player = lookupOpponent(username);

		if(wasDrawedJustBefore) {
			player->drawSingleCardInHandCardsLocal(card, drawCardStack);
			player->playCardFromHandCardsAfterDrawTime(card, playCardStack, canChangeColor(card));
		} else {
			player->playCardFromHandCards(card, playCardStack, canChangeColor(card));
		}

		if(canChangeColor(card)) {
			if(newCardIndex == CardIndex::NULLINDEX) throw std::runtime_error("Can't set NULLINDEX as new card index");
			this->indexForNextCard = newCardIndex;
		} else {
			this->indexForNextCard = card.getCardIndex();
		}

		setNextOrNextButOneOnTurnLocal(card);

		playerHasToDrawCards(userOnTurn, cardsToDraw);
	}
	void ProxyMauMauGame::drawCardAndSetNextPlayerOnTurnLocal(std::string username) {
		std::shared_ptr<ProxyPlayer> player = lookupOpponent(username);
		player->drawSingleCardInHandCardsLocal(Card::NULLCARD, drawCardStack);
		tryRebalanceCardStacks();

		setNextPlayerOnTurnLocal();
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
		this->userOnTurn->onEndTurn();
		this->userOnTurn = player;
		this->userOnTurn->onStartTurn();
	}
	
	void ProxyMauMauGame::playerHasToDrawCards(std::shared_ptr<ProxyPlayer> player, std::size_t amountOfCards) {
		if(amountOfCards == 0) return;

		auto cards = Card::getVectorWithCards(Card::NULLCARD, amountOfCards);
		playerHasToDrawCards(player, cards);
	}

	void ProxyMauMauGame::playerHasToDrawCards(std::shared_ptr<ProxyPlayer> player, const std::vector<Card>& cards) {
		player->drawMultipleCardsInHandCardsAfterCardPlayTimeLocal(cards, drawCardStack);
		tryRebalanceCardStacks();
	}

	void ProxyMauMauGame::tryRebalanceCardStacks() {
		if(drawCardStack.getSize() <= 3) {
			// TODO
			throw std::runtime_error("NOT IMPLEMENTED YET");
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
}
