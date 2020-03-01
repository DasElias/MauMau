#include "ProxyMauMauGameData.h"
#include <shared/model/PlayerNotFoundException.h>

#include <shared/model/MauMauCardValueMeanings.h>


#include <iostream>
#include <shared/model/CardAnimationDuration.h>
#include <shared/utils/ThreadUtils.h>
#include <shared/model/TimeToSetNextPlayerOnTurnDuration.h>
#include <shared/model/MinDrawCardStackSize.h>
#include <shared/utils/Logger.h>

namespace card {
	ProxyMauMauGameData::ProxyMauMauGameData(std::vector<std::shared_ptr<ParticipantOnClient>> allParticipantsInclLocal, std::shared_ptr<ParticipantOnClient> localParticipant, std::vector<int> handCards, int startCard, RoomOptions& roomOptions, std::function<void(std::shared_ptr<ProxyPlayer>)> onTurnEnd) :
			drawCardStack(std::make_unique<CardStack>()),
			playCardStack(std::make_unique<CardStack>()),
			indexForNextCard(Card(startCard).getCardIndex()),
			roomOptions(roomOptions),
			userOnTurn(nullptr),
			onTurnEndCallback(onTurnEnd) {

		// initialize players
		for(auto& o : allParticipantsInclLocal) {
			if(o == localParticipant) {
				this->localPlayer = std::make_shared<LocalPlayer>(localParticipant, gameInformation);
				this->allPlayers.push_back(localPlayer);
			} else {
				auto player = std::make_shared<ProxyPlayer>(o, gameInformation);
				this->opponents.push_back(player);
				this->allPlayers.push_back(player);
			}
		}

		// initialize other player's hand cards and draw card stack
		initStartCards(handCards, Card(startCard));

		setLocalPlayerAtTheBeginOfPlayersVector();
	}

	ProxyMauMauGameData::~ProxyMauMauGameData() {
		threadUtils_removeCallbacksWithKey(this);
	}

	void ProxyMauMauGameData::initStartCards(const std::vector<int>& handCardNumbersOfLocalPlayer, Card cardOnPlayStack) {
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

	bool ProxyMauMauGameData::isLocalPlayerOnTurn() const {
		return userOnTurn == localPlayer;
	}

	bool ProxyMauMauGameData::areAllPreviousCardTransactionsCompleted() const {
		if(! playCardStack.getCardAnimations().empty()) return false;
		if(! localPlayer->getTempCardStack().getCardAnimations().empty()) return false;

		for(auto& player : this->allPlayers) {
			if(! player->getCardStack().getCardAnimations().empty()) return false;
		}
		
		return true;
	}

	bool ProxyMauMauGameData::canChangeColor(Card playedCard) const {
		return !hasGameEnded() && roomOptions.getOption(Options::CHOOSE_COLOR_ON_JACK) && playedCard.getValue() == CHANGE_COLOR_VALUE;
	}

	bool ProxyMauMauGameData::canSkipPlayer(Card card) const {
		return roomOptions.getOption(Options::SKIP_ON_EIGHT) && card.getValue() == SKIP_VALUE;
	}

	std::size_t ProxyMauMauGameData::getAmountsOfCardsToDrawForNextPlayer(Card playedCard) const {
		if(! roomOptions.getOption(Options::DRAW_TWO_ON_SEVEN)) return false;
		return (playedCard.getValue() == DRAW_2_VALUE) ? 2 : 0;
	}

	void ProxyMauMauGameData::playCardFromHandCards(std::shared_ptr<ProxyPlayer> player, Card card, CardIndex newCardIndex, int delay) {
		throwIfGameHasEnded();
		field_wasCardPlayed = true;
		player->playCardFromHandCardsAfterDelay(card, playCardStack, delay);
		updateCardIndex(card, newCardIndex);
		updateDirection(card);
	}
	void ProxyMauMauGameData::playCardFromLocalPlayerTempCards(CardIndex newCardIndex, int delay) {
		auto drawnCardOrNone = localPlayer->getCardInTempStack();
		if(!drawnCardOrNone.has_value()) {
			throw std::runtime_error("LocalPlayer hasn't drawn a card in temp cards");
		}
		auto drawnCard = *drawnCardOrNone;

		throwIfGameHasEnded();
		field_wasCardPlayed = true;
		localPlayer->playCardFromTempCardStackLocal(playCardStack);
		updateCardIndex(drawnCard, newCardIndex);
		updateDirection(drawnCard);
	}
	void ProxyMauMauGameData::updateCardIndex(Card playedCard, CardIndex newCardIndex) {
		if(newCardIndex == CardIndex::NULLINDEX) {
			this->indexForNextCard = playedCard.getCardIndex();
		} else {
			this->indexForNextCard = newCardIndex;
		}
	}
	void ProxyMauMauGameData::updateDirection(Card playedCard) {
		if(playedCard.getValue() == CHANGE_DIRECTION_VALUE && roomOptions.getOption(Options::DIRECTION_CHANGE_ON_NINE)) {
			if(direction == Direction::CW) direction = Direction::CCW;
			else direction = Direction::CW;
		}
	}
	void ProxyMauMauGameData::drawInHandCardsFromCardStack(std::shared_ptr<ProxyPlayer> player, Card card) {
		throwIfGameHasEnded();
		field_wasCardDrawnIntoHandCards = true;
		player->drawSingleCardInHandCardsLocal(card, drawCardStack);
	}

	void ProxyMauMauGameData::drawInHandCardsFromTempCards() {
		throwIfGameHasEnded();
		field_wasCardDrawnIntoHandCards = true;
		localPlayer->sortDrawnCardIntoHandCardsLocal();
	}

	void ProxyMauMauGameData::drawInLocalPlayerTempCards() {
		throwIfGameHasEnded();
		Card cardToDraw = getDrawCardForNextPlayer();
		localPlayer->drawSingleCardInTempCardStackLocal(cardToDraw, drawCardStack);
	}

	void ProxyMauMauGameData::throwIfGameHasEnded() {
		if(hasGameEnded()) {
			throw std::runtime_error("Can't perform action when the game has already ended.");
		}
	}
	const CardAnimator& ProxyMauMauGameData::getDrawStack() const {
		return drawCardStack;
	}

	const CardAnimator& ProxyMauMauGameData::getPlayStack() const {
		return playCardStack;
	}

	std::shared_ptr<LocalPlayer> ProxyMauMauGameData::getLocalPlayer() {
		return localPlayer;
	}

	std::shared_ptr<ProxyPlayer> ProxyMauMauGameData::getPlayerOnTurn() {
		return userOnTurn;
	}

	std::vector<std::shared_ptr<ProxyPlayer>> ProxyMauMauGameData::getOpponents() {
		return opponents;
	}

	void ProxyMauMauGameData::removeOpponentLocal(std::shared_ptr<ParticipantOnClient> participant) {
		std::string participantUsername = participant->getUsername();
		appendMessage(participantUsername + u8" verließ das Spiel.");

		if(hasGameEnded()) return;
		auto player = lookupOpponent(participantUsername);

		if(userOnTurn == player) {
			setNextPlayerOnTurnLocal();
		}

		opponents.erase(std::find(opponents.begin(), opponents.end(), player));
		allPlayers.erase(std::find(allPlayers.begin(), allPlayers.end(), player));	
	}

	bool ProxyMauMauGameData::checkIfIsOpponent(std::string username) const {
		for(auto& o : opponents) {
			if(o->getUsername() == username) return true;
		}
		return false;
	}

	bool ProxyMauMauGameData::checkIfIsParticipant(std::string username) const {
		for(auto& o : allPlayers) {
			if(o->getUsername() == username) return true;
		}
		return false;
	}

	std::shared_ptr<ProxyPlayer> ProxyMauMauGameData::lookupPlayer(std::string username) {
		for(auto& o : allPlayers) {
			if(o->getUsername() == username) return o;
		}

		throw PlayerNotFoundException("\"" + username + "\" is not a player.");
	}
	std::shared_ptr<ProxyPlayer> ProxyMauMauGameData::lookupOpponent(std::string username) {
		for(auto& o : opponents) {
			if(o->getUsername() == username) return o;
		}

		throw PlayerNotFoundException("\"" + username + "\" is not an opponent.");
	}
	bool ProxyMauMauGameData::hasGameEnded() const {
		return getWinnerOrNull() != nullptr;
	}
	bool ProxyMauMauGameData::hasInitialCardBeenDistributed() const {
		return field_hasInitialCardsBeenDistributed;
	}
	std::shared_ptr<ProxyPlayer> ProxyMauMauGameData::getWinnerOrNull() const {
		// if there's only one player ingame, he has won the game
		if(allPlayers.size() == 1) return allPlayers[0];

		// if the distribution of the initial cards hasn't been finished, no player can have won 
		if(! field_hasInitialCardsBeenDistributed) return nullptr;

		for(auto& player : allPlayers) {
			if(player->getCardStack().isEmptyAndNoPendingTransactions()) {
				return player;
			}
		}
		return nullptr;
	}

	const RoomOptions& ProxyMauMauGameData::getOptions() const {
		return roomOptions;
	}
	
	void ProxyMauMauGameData::abortTurnOnTimeExpires(const std::vector<Card>& cardsToDraw) {
		if(isLocalPlayerOnTurn() && cardsToDraw[0] == Card::NULLCARD) {
			throw std::runtime_error("Local player can't draw NULLCARD on time expire!");
		}

		playerHasToDrawCards(userOnTurn, cardsToDraw);

		setNextPlayerOnTurnLocal();
		appendMessage("Zeit ist abgelaufen!");
	}
	void ProxyMauMauGameData::onSuccessfulMau(std::shared_ptr<ProxyPlayer> player) {
		player->onSuccessfulMau();
	}
	void ProxyMauMauGameData::onMauPunishment(std::shared_ptr<ProxyPlayer> punishedPlayer, std::vector<Card> cardsToDraw, MauPunishmentCause cause) {
		int delay = (cause == MauPunishmentCause::NO_MAU_RECEIVED) ? PLAY_DURATION_MS : 0;
		playerHasToDrawCards(punishedPlayer, cardsToDraw, delay);

		std::string punishedUsername = punishedPlayer->getUsername();
		appendMauPunishmentMessage(punishedUsername, cause);

		if(punishedPlayer == localPlayer) {
			localPlayer->onMauFailure();
		}
	}
	bool ProxyMauMauGameData::wasCardDrawnIntoHandCardsThisTurn() const {
		return field_wasCardDrawnIntoHandCards;
	}
	bool ProxyMauMauGameData::wasCardPlayedThisTurn() const {
		return field_wasCardPlayed;
	}
	void ProxyMauMauGameData::appendMauPunishmentMessage(std::string punishedUsername, MauPunishmentCause cause) {
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
	void ProxyMauMauGameData::setNextOrNextButOneOnTurnLocal(Card playedCard) {
		if(canSkipPlayer(playedCard)) setNextButOnePlayerOnTurnLocal();
		else setNextPlayerOnTurnLocal();
	}
	void ProxyMauMauGameData::setNextPlayerOnTurnLocal() {
		std::shared_ptr<ProxyPlayer> nextPlayer = getNextPlayer(userOnTurn);
		setOnTurnLocal(nextPlayer);
	}
	void ProxyMauMauGameData::setNextButOnePlayerOnTurnLocal() {
		std::shared_ptr<ProxyPlayer> nextPlayer = getNextPlayer(userOnTurn);
		nextPlayer->onSkip();
		std::shared_ptr<ProxyPlayer> nextButOnePlayer = getNextPlayer(nextPlayer);
		setOnTurnLocal(nextButOnePlayer);
	}
	std::shared_ptr<ProxyPlayer> ProxyMauMauGameData::getNextPlayer(std::shared_ptr<ProxyPlayer> playerOnTurn) {
		auto playerOnTurnIter = std::find(allPlayers.begin(), allPlayers.end(), playerOnTurn);

		if(direction == Direction::CW) {
			playerOnTurnIter++;
			if(playerOnTurnIter == allPlayers.end()) playerOnTurnIter = allPlayers.begin();
		} else {
			if(playerOnTurnIter == allPlayers.begin()) playerOnTurnIter = allPlayers.end();
			playerOnTurnIter--;
		}	

		return *playerOnTurnIter;
	}
	void ProxyMauMauGameData::setOnTurnLocal(std::shared_ptr<ProxyPlayer> player) {	
		int delayToSetNextPlayerOnTurn = getTimeToSetNextPlayerOnTurn(playCardStack.getSizeInclPendingTransactions(), playCardStack.getLastInclAnimations(), field_wasCardPlayed, field_wasCardDrawnIntoHandCards, roomOptions);
		int delayToFreezeAnimation = getTimeToEndCurrentTurn(playCardStack.getSizeInclPendingTransactions(), playCardStack.getLastInclAnimations(), field_wasCardPlayed, field_wasCardDrawnIntoHandCards);
		// TODO FIX: Wenn Bedingung erfüllt, delayToFreezeAnimation von delayToSetNextPlayerOnTurn abziehen
		if(field_wasCardDrawnIntoHandCards && field_wasCardPlayed && userOnTurn == localPlayer) {
			// we don't have to take the time for drawing the card into consideration
			delayToFreezeAnimation = 0;
		}


		std::shared_ptr<ProxyPlayer> lastUserOnTurn = this->userOnTurn;
		this->userOnTurn->onEndTurn();
		onTurnEndCallback(this->userOnTurn);
		this->userOnTurn = player;

		field_wasCardDrawnIntoHandCards = false;
		field_wasCardPlayed = false;
		threadUtils_invokeIn(delayToSetNextPlayerOnTurn, [this, player, lastUserOnTurn]() {			
			lastUserOnTurn->endRemainingTimeAnimation();
			this->userOnTurn->onStartTurn();
		});
		threadUtils_invokeIn(delayToFreezeAnimation, [this, lastUserOnTurn]() {
			lastUserOnTurn->freezeRemainingTimeAnimation();
		});
	}

	void ProxyMauMauGameData::setInitialPlayerOnTurnLocal(std::shared_ptr<ProxyPlayer> player, Card nextCardOnDrawStack) {
		this->userOnTurn = player;
		userOnTurn->onStartTurn();

		this->drawCardForNextPlayer = Card(nextCardOnDrawStack);
		field_hasInitialCardsBeenDistributed = true;
	}

	std::optional<CardIndex> ProxyMauMauGameData::getCardIndexForNextCardOrNone() const {
		if(playCardStack.getSize() <= 1 || playCardStack.getLast().getValue() != CHANGE_COLOR_VALUE) return std::nullopt;
		else return indexForNextCard;
	}

	CardIndex ProxyMauMauGameData::getCardIndexForNextCard() const {
		return indexForNextCard;
	}

	void ProxyMauMauGameData::setDrawCardForNextPlayer(Card c) {
		drawCardForNextPlayer = c;
	}

	Card ProxyMauMauGameData::getDrawCardForNextPlayer() const {
		return drawCardForNextPlayer;
	}
	
	void ProxyMauMauGameData::playerHasToDrawCards(std::shared_ptr<ProxyPlayer> player, std::size_t amountOfCards, int delayMs) {
		if(amountOfCards == 0) return;

		auto cards = Card::getVectorWithCards(Card::NULLCARD, amountOfCards);
		playerHasToDrawCards(player, cards, delayMs);
	}

	void ProxyMauMauGameData::playerHasToDrawCards(std::shared_ptr<ProxyPlayer> player, const std::vector<Card>& cards, int delayMs) {
		player->drawMultipleCardsInHandCardsAfterDelay(cards, drawCardStack, delayMs);
		tryRebalanceCardStacks();
	}

	void ProxyMauMauGameData::tryRebalanceCardStacks() {
		while(drawCardStack.getSize() <= MIN_DRAW_CARD_STACK_SIZE && playCardStack.getSize() > 1) {
			drawCardStack.addFirstCardFrom(Card::NULLCARD, playCardStack, REBALANCE_DURATION, 0);
		}
		if(drawCardStack.getSize() < MIN_DRAW_CARD_STACK_SIZE) {
			for(int i = 1; i <= Card::MAX_CARDS; i++) {
				drawCardStack.addFromPlainAtPosition(0, Card(i));
			}
		} 
	}
	void ProxyMauMauGameData::setLocalPlayerAtTheBeginOfPlayersVector() {
		while(this->allPlayers[0] != localPlayer) {
			allPlayers.push_back(allPlayers[0]);
			allPlayers.erase(allPlayers.begin());

			opponents.push_back(opponents[0]);
			opponents.erase(opponents.begin());
		}
	}
	void ProxyMauMauGameData::appendMessage(std::string content) {
		messageQueue.appendMessage(content);
	}
	const MessageQueue ProxyMauMauGameData::getMessageQueue() const {
		return messageQueue;
	}
	
}
