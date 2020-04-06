#include "ProxyMauMauGameData.h"
#include <shared/model/PlayerNotFoundException.h>

#include <shared/model/MauMauCardValueMeanings.h>


#include <iostream>
#include <shared/model/CardAnimationDuration.h>
#include <shared/utils/ThreadUtils.h>
#include <shared/model/TimeToSetNextPlayerOnTurnDuration.h>
#include <shared/model/MinDrawCardStackSize.h>
#include <shared/utils/Logger.h>
#include <egui/input/IOHandler.h>

namespace card {
	static void setLocalPlayerAtTheBeginOfVector(std::vector<std::shared_ptr<ParticipantOnClient>> allParticipantsInclLocal, std::shared_ptr<ParticipantOnClient> localParticipant) {
		while(allParticipantsInclLocal[0] != localParticipant) {
			allParticipantsInclLocal.push_back(allParticipantsInclLocal[0]);
			allParticipantsInclLocal.erase(allParticipantsInclLocal.begin());
		}
	}

	ProxyMauMauGameData::ProxyMauMauGameData(std::vector<std::shared_ptr<ParticipantOnClient>> allParticipantsInclLocal, std::shared_ptr<ParticipantOnClient> localParticipant, std::vector<int> handCards, int startCard, RoomOptions& roomOptions, std::function<void(std::shared_ptr<ProxyPlayer>)> onTurnEnd) :
			drawCardStack(std::make_unique<CardStack>()),
			playCardStack(std::make_unique<CardStack>()),
			indexForNextCard(Card(startCard).getCardIndex()),
			playerList({}, nullptr),	// will be initialized later correctly
			roomOptions(roomOptions),
			onTurnEndCallback(onTurnEnd) {

		setLocalPlayerAtTheBeginOfVector(allParticipantsInclLocal, localParticipant);

		// initialize players
		for(auto& o : allParticipantsInclLocal) {
			if(o == localParticipant) {
				this->localPlayer = std::make_shared<LocalPlayer>(localParticipant);
				playerList.appendPlayer(localPlayer);
			} else {
				auto player = std::make_shared<ProxyPlayer>(o);
				this->opponents.push_back(player);
				playerList.appendPlayer(player);
			}
		}

		// initialize other player's hand cards and draw card stack
		initStartCards(handCards, Card(startCard));
	}


	ProxyMauMauGameData::~ProxyMauMauGameData() {
		threadUtils_removeCallbacksWithKey(this);
	}

	void ProxyMauMauGameData::initStartCards(const std::vector<int>& handCardNumbersOfLocalPlayer, Card cardOnPlayStack) {
		// init draw card stack
		drawCardStack.addFromPlain(Card::NULLCARD, Card::MAX_CARDS * roomOptions.getOption(Options::AMOUNT_OF_START_CARD_DECKS));

		std::size_t cardsPerPlayer = handCardNumbersOfLocalPlayer.size();
		std::vector<Card> nullHandCards = Card::getVectorWithCards(Card::NULLCARD, cardsPerPlayer);
		for(std::size_t i = 0; i < opponents.size(); i++) {
			auto& o = opponents[i];
			o->initHandCards(nullHandCards, drawCardStack, i);
		}
		
		std::vector<Card> handCardsOfLocalPlayer = Card::getVectorFromCardNumber(handCardNumbersOfLocalPlayer);
		localPlayer->initHandCards(handCardsOfLocalPlayer, drawCardStack, opponents.size());

		// init play card stack
		playCardStack.addLastCardFrom(cardOnPlayStack, drawCardStack, INITIAL_DRAW_DURATION_PLAYCARDSTACK_MS, getDelayUntilPlayStackCanBeInitialized(playerList.getAmountOfPlayers(), handCardNumbersOfLocalPlayer.size()));
	}

	void ProxyMauMauGameData::addCardsToDrawOnPassDueToPlusTwo(std::size_t amountOfNullcards) {
		// we change all cards in cardsToDrawOnPassDueToPlusTwo in NULLCARDS
		for(int i = 0; i < cardsToDrawOnPassDueToPlusTwo.size(); i++) {
			cardsToDrawOnPassDueToPlusTwo[i] = Card::NULLCARD;
		}

		// add amountOfNullcards
		for(int i = 0; i < amountOfNullcards; i++) {
			cardsToDrawOnPassDueToPlusTwo.push_back(Card::NULLCARD);
		}
	}

	void ProxyMauMauGameData::setCardsToDrawOnPassDueToPlusTwo(std::vector<Card> cards) {
		this->cardsToDrawOnPassDueToPlusTwo = cards;
	}

	bool ProxyMauMauGameData::isInDrawTwoState() const {
		return cardsToDrawOnPassDueToPlusTwo.size() > 0;
	}

	std::size_t ProxyMauMauGameData::getSizeOfCardsToDrawDueToPlusTwo() const {
		return cardsToDrawOnPassDueToPlusTwo.size();
	}

	bool ProxyMauMauGameData::isLocalPlayerOnTurn() const {
		return playerList.isPlayerOnTurn(localPlayer);
	}

	bool ProxyMauMauGameData::areAllPreviousCardTransactionsCompleted() const {
		if(! playCardStack.getCardAnimations().empty()) return false;
		if(! localPlayer->getTempCardStack().getCardAnimations().empty()) return false;

		for(auto& player : playerList) {
			if(! player->getCardStack().getCardAnimations().empty()) return false;
		}
		
		return true;
	}

	bool ProxyMauMauGameData::isReadyToPerformLocalPlayerTurn() const {
		// we check localPlayer->isRemainingTimeAnimationActive because of the special case that another player draws and plays a card in the same turn
		// in this case, the local player would be already on turn even though the card played before is still in the other player's hand card for a short amount of time
		return isLocalPlayerOnTurn() && areAllPreviousCardTransactionsCompleted() && localPlayer->isRemainingTimeAnimationActive() && !hasGameEnded();
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
		clearPermanentMessagesIfGameHasEnded();
	}
	void ProxyMauMauGameData::playCardFromLocalPlayerHandCards(std::size_t indexInHandCards, CardIndex newCardIndex, int delayMs) {
		Card card = localPlayer->getCardStack().get(indexInHandCards);
		throwIfGameHasEnded();
		field_wasCardPlayed = true;
		localPlayer->playCardFromHandCardsAfterDelay(indexInHandCards, playCardStack, delayMs);
		updateCardIndex(card, newCardIndex);
		updateDirection(card);
		clearPermanentMessagesIfGameHasEnded();
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
		clearPermanentMessagesIfGameHasEnded();
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
			appendMessage("Richtungswechsel!");
			if(direction == Direction::CW) direction = Direction::CCW;
			else direction = Direction::CW;
		}
	}
	void ProxyMauMauGameData::drawInHandCardsFromCardStack(std::shared_ptr<ProxyPlayer> player, Card card) {
		throwIfGameHasEnded();
		field_wasCardDrawnIntoHandCards = true;
		player->drawCardInHandCards(card, drawCardStack);
		tryRebalanceCardStacks();
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
		tryRebalanceCardStacks();
	}

	void ProxyMauMauGameData::throwIfGameHasEnded() {
		if(hasGameEnded()) {
			throw std::runtime_error("Can't perform action when the game has already ended.");
		}
	}
	void ProxyMauMauGameData::clearPermanentMessagesIfGameHasEnded() {
		if(hasGameEnded()) {
			clearPermanentMessages();
		}
	}
	void ProxyMauMauGameData::clearPermanentMessages() {
		messageQueue.removeMessagesWithKey(skipStateMessageKey);
		messageQueue.removeMessagesWithKey(drawTwoMessageKey);
		
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
		return playerList.getPlayerOnTurn();
	}

	std::vector<std::shared_ptr<ProxyPlayer>> ProxyMauMauGameData::getOpponents() {
		return opponents;
	}

	void ProxyMauMauGameData::removeOpponentLocal(std::shared_ptr<ParticipantOnClient> participant) {
		std::string participantUsername = participant->getUsername();
		appendMessage(participantUsername + u8" verließ das Spiel.");
		clearPermanentMessagesIfGameHasEnded();

		if(hasGameEnded()) return;
		auto player = lookupOpponent(participantUsername);

		if(playerList.isPlayerOnTurn(player)) {
			setNextPlayerOnTurnLocal();
		}

		opponents.erase(std::find(opponents.begin(), opponents.end(), player));
		playerList.removePlayer(player);
	}

	bool ProxyMauMauGameData::checkIfIsOpponent(std::string username) const {
		for(auto& o : opponents) {
			if(o->getUsername() == username) return true;
		}
		return false;
	}

	bool ProxyMauMauGameData::checkIfIsParticipant(std::string username) const {
		for(auto& o : playerList) {
			if(o->getUsername() == username) return true;
		}
		return false;
	}

	std::shared_ptr<ProxyPlayer> ProxyMauMauGameData::lookupPlayer(std::string username) {
		for(auto& o : playerList) {
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
		const auto& allPlayers = playerList.getAllPlayers();

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
	
	void ProxyMauMauGameData::abortTurnOnTimeExpires(const std::vector<Card>& cardsToDraw, int amountOfCardsToDrawBefore) {
		if(isLocalPlayerOnTurn() && cardsToDraw[0] == Card::NULLCARD) {
			throw std::runtime_error("Local player can't draw NULLCARD on time expire!");
		}

		int delayMs = amountOfCardsToDrawBefore * DRAW_MULTIPLE_DELAY_BETWEEN_CARDS_MS;
		playerHasToDrawCards(playerList.getPlayerOnTurn(), cardsToDraw, delayMs);

		setNextPlayerOnTurnLocal();
		appendMessage("Zeit ist abgelaufen!");
	}
	void ProxyMauMauGameData::onSuccessfulMau(std::shared_ptr<ProxyPlayer> player) {
		if(roomOptions.getOption(Options::HAVE_TO_MAU)) {
			player->onSuccessfulMau();
		}
	}
	void ProxyMauMauGameData::onMauPunishment(std::shared_ptr<ProxyPlayer> punishedPlayer, std::vector<Card> cardsToDraw, MauPunishmentCause cause) {
		if(! roomOptions.getOption(Options::HAVE_TO_MAU)) return;

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
	void ProxyMauMauGameData::setNextPlayerOnTurnAndUpdateSkipAndDrawTwoState(Card playedCard) {
		if(roomOptions.getOption(Options::PASS_SKIP)) {
			setNextPlayerOnTurnLocal();
			if(canSkipPlayer(playedCard)) {
				auto userOnTurn = playerList.getPlayerOnTurn();
				userOnTurn->setSkipState();
				if(userOnTurn == localPlayer) {
					messageQueue.appendMessagePermanently("Du wurdest ausgelassen. Spiele eine 8 oder passe.", skipStateMessageKey);
				}
			}
		} else {
			if(canSkipPlayer(playedCard)) setNextButOnePlayerOnTurnLocal();
			else setNextPlayerOnTurnLocal();
		}
		
		if(roomOptions.getOption(Options::PASS_DRAW_TWO) && isInDrawTwoState() && playerList.isPlayerOnTurn(localPlayer)) {
			messageQueue.appendMessagePermanently("Spiele eine 7 oder passe, um "+std::to_string(getSizeOfCardsToDrawDueToPlusTwo())+" Karten zu ziehen.", drawTwoMessageKey);
		}

		clearPermanentMessagesIfGameHasEnded();
	}
	void ProxyMauMauGameData::setNextPlayerOnTurnLocal() {
		std::shared_ptr<ProxyPlayer> nextPlayer = playerList.getNextPlayerOnTurn(direction);
		setOnTurnLocal(nextPlayer);
	}
	void ProxyMauMauGameData::setNextButOnePlayerOnTurnLocal() {
		std::shared_ptr<ProxyPlayer> nextPlayer = playerList.getNextPlayerOnTurn(direction);
		nextPlayer->startSkippedAnimation();
		std::shared_ptr<ProxyPlayer> nextButOnePlayer = playerList.getNextPlayerOnTurn(direction, nextPlayer);
		setOnTurnLocal(nextButOnePlayer);
	}
	void ProxyMauMauGameData::setOnTurnLocal(std::shared_ptr<ProxyPlayer> player) {	
		int cardsToDrawDueToPlusTwoAmount = 0;

		// if field_wasCardPlayed would be true, the player on turn would have passed skip to the next player
		if(playerList.getPlayerOnTurn()->isInSkipState() && !field_wasCardPlayed) {
			playerList.getPlayerOnTurn()->startSkippedAnimation();
		}
		if(isInDrawTwoState() && !field_wasCardPlayed) {
			if(! roomOptions.getOption(Options::PASS_DRAW_TWO)) throw std::runtime_error("It isn't possible that a player has to draw cards due to a plus two, even though no (+2)-card was played when +2 cards can't be passed on.");
			cardsToDrawDueToPlusTwoAmount = cardsToDrawOnPassDueToPlusTwo.size();
			playerHasToDrawCards(playerList.getPlayerOnTurn(), cardsToDrawOnPassDueToPlusTwo);
		}
		if(isInDrawTwoState() && !roomOptions.getOption(Options::PASS_DRAW_TWO)) {
			cardsToDrawDueToPlusTwoAmount = cardsToDrawOnPassDueToPlusTwo.size();
			// we actually draw the cards when the new player was set on turn (at the end of the method)
		}

		// we don't have to consider this delay if PASS_DRAW_TWO is disabled
		int delayForDrawDueToPlusTwo = PLAY_DURATION_MS + ((field_wasCardDrawnIntoHandCards) ? (DRAW_DURATION_MS + DELAY_BETWEEN_DRAW_AND_PLAY) : 0);
		int delayToSetNextPlayerOnTurn = getTimeToSetNextPlayerOnTurn(field_wasCardPlayed, field_wasCardDrawnIntoHandCards, cardsToDrawDueToPlusTwoAmount);
		int delayToFreezeAnimation = getTimeToEndCurrentTurn(field_wasCardPlayed, field_wasCardDrawnIntoHandCards);
		if(field_wasCardDrawnIntoHandCards && field_wasCardPlayed && playerList.isPlayerOnTurn(localPlayer)) {
			// we don't have to take the time for drawing the card into consideration
			delayToFreezeAnimation = 0;
		}

		std::shared_ptr<ProxyPlayer> lastUserOnTurn = playerList.getPlayerOnTurn();
		lastUserOnTurn->onEndTurn();
		onTurnEndCallback(lastUserOnTurn);
		playerList.setPlayerOnTurn(player);

		clearPermanentMessages();
		if(! field_wasCardPlayed) {
			// if field_wasCardPlayed would be true, the player on turn would have +2 to the next player
			this->cardsToDrawOnPassDueToPlusTwo.clear();
		}
		field_wasCardDrawnIntoHandCards = false;
		field_wasCardPlayed = false;
		threadUtils_invokeIn(delayToSetNextPlayerOnTurn, this, [this, player, lastUserOnTurn]() {			
			lastUserOnTurn->endRemainingTimeAnimation();
			player->onStartTurn();
		});
		threadUtils_invokeIn(delayToFreezeAnimation, this, [this, lastUserOnTurn]() {
			lastUserOnTurn->freezeRemainingTimeAnimation();
		});

		if(isInDrawTwoState() && !roomOptions.getOption(Options::PASS_DRAW_TWO)) {
			playerHasToDrawCards(player, cardsToDrawOnPassDueToPlusTwo, delayForDrawDueToPlusTwo);
			cardsToDrawOnPassDueToPlusTwo.clear();
		}
	}

	void ProxyMauMauGameData::setInitialPlayerOnTurnLocal(std::shared_ptr<ProxyPlayer> player, Card nextCardOnDrawStack) {
		playerList.setPlayerOnTurn(player);
		player->onStartTurn();

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
		for(const Card& c : cards) {
			player->drawCardInHandCards(c, drawCardStack, delayMs);
			tryRebalanceCardStacks();

			delayMs += DRAW_MULTIPLE_DELAY_BETWEEN_CARDS_MS;
		}
	}

	void ProxyMauMauGameData::tryRebalanceCardStacks() {
		// move card from play to draw stack while (there are too little cards on draw card stack and at least one card on play card stack) or (there are too much cards on play card stacks)
		while((drawCardStack.getAvailableCardsSize() <= MIN_DRAW_CARD_STACK_SIZE && playCardStack.getAvailableCardsSize() > 1) || playCardStack.getAvailableCardsSize() > Card::MAX_CARDS) {
			drawCardStack.addFirstCardFromImmediately(Card::NULLCARD, playCardStack, REBALANCE_DURATION);
		}

		if(drawCardStack.getAvailableCardsSize() == 0 || drawCardStack.getAvailableCardsSize() + drawCardStack.getNumberOfIncomingAnimations() < MIN_DRAW_CARD_STACK_SIZE) {
			for(int i = 1; i <= Card::MAX_CARDS; i++) {
				drawCardStack.addFromPlainAtPosition(0, Card(i));
			}
		}
	}
	void ProxyMauMauGameData::appendMessage(std::string content) {
		messageQueue.appendMessage(content);
	}
	const MessageQueue& ProxyMauMauGameData::getMessageQueue() const {
		return messageQueue;
	}

	MessageQueue& ProxyMauMauGameData::getMessageQueue() {
		return messageQueue;
	}
	
}
