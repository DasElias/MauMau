#include "ProxyMauMauGameData.h"
#include <shared/model/PlayerNotFoundException.h>

#include <shared/model/MauMauCardValueMeanings.h>
#include "MauMauPunishmentMessageGenerator.h"

#include <iostream>
#include <shared/model/CardAnimationDuration.h>
#include <shared/utils/ThreadUtils.h>
#include <shared/model/TimeToSetNextPlayerOnTurnDuration.h>
#include <shared/model/MinDrawCardStackSize.h>
#include <shared/utils/Logger.h>
#include <egui/input/IOHandler.h>

namespace card {
	ProxyMauMauGameData::ProxyMauMauGameData(std::vector<std::shared_ptr<ParticipantOnClient>> allParticipantsInclLocal, std::shared_ptr<ParticipantOnClient> localParticipant, std::vector<int> handCards, int startCard, RoomOptions& roomOptions) :
			drawCardStack(std::make_unique<CardStack>()),
			playCardStack(std::make_unique<CardStack>()),
			indexForNextCard(Card(startCard).getCardIndex()),
			playerList(allParticipantsInclLocal, localParticipant),
			roomOptions(roomOptions) {


		// initialize other player's hand cards and draw card stack
		initStartCards(handCards, Card(startCard));
	}


	ProxyMauMauGameData::~ProxyMauMauGameData() {
		threadUtils_removeCallbacksWithKey(this);
	}

	void ProxyMauMauGameData::initStartCards(const std::vector<int>& handCardNumbersOfLocalPlayer, Card cardOnPlayStack) {
		// init draw card stack
		drawCardStack.addFromPlain(Card::NULLCARD, Card::MAX_CARDS * std::size_t(roomOptions.getOption(Options::AMOUNT_OF_START_CARD_DECKS)));

		// init player's start cards
		auto opponents = playerList.getOpponents();
		LocalPlayer& localPlayer = playerList.getLocalPlayer();

		std::size_t cardsPerPlayer = handCardNumbersOfLocalPlayer.size();
		std::vector<Card> nullHandCards = Card::getVectorWithCards(Card::NULLCARD, cardsPerPlayer);
		for(std::size_t i = 0; i < opponents.size(); i++) {
			auto& o = opponents[i];
			o->initHandCards(nullHandCards, drawCardStack, i);
		}
		
		std::vector<Card> handCardsOfLocalPlayer = Card::getVectorFromCardNumber(handCardNumbersOfLocalPlayer);
		localPlayer.initHandCards(handCardsOfLocalPlayer, drawCardStack, opponents.size());

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

	std::size_t ProxyMauMauGameData::getSizeOfCardsToDrawByCurrentPlayerDueToPlusTwo() const {
		if(!field_wasCardPlayed || !roomOptions.getOption(Options::PASS_DRAW_TWO)) {
			return cardsToDrawOnPassDueToPlusTwo.size();
		} else {
			return 0;
		}
	}

	bool ProxyMauMauGameData::isLocalPlayerOnTurn() const {
		return playerList.isLocalPlayerOnTurn();
	}

	bool ProxyMauMauGameData::areAllPreviousCardTransactionsCompleted() const {
		const LocalPlayer& localPlayer = playerList.getLocalPlayer();

		if(! playCardStack.getCardAnimations().empty()) return false;
		if(! localPlayer.getTempCardStack().getCardAnimations().empty()) return false;

		for(auto& player : playerList.getPlayers()) {
			if(! player.getCardStack().getCardAnimations().empty()) return false;
		}
		
		return true;
	}

	bool ProxyMauMauGameData::isReadyToPerformLocalPlayerTurn() const {
		// we check localPlayer->isRemainingTimeAnimationActive because of the special case that another player draws and plays a card in the same turn
		// in this case, the local player would be already on turn even though the card played before is still in the other player's hand card for a short amount of time
		return isLocalPlayerOnTurn() && areAllPreviousCardTransactionsCompleted() && playerList.getLocalPlayer().isRemainingTimeAnimationActive() && !hasGameEnded();
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

	void ProxyMauMauGameData::playCardFromHandCards(ProxyPlayer& player, Card card, CardIndex newCardIndex, int delay) {
		throwIfGameHasEnded();
		field_wasCardPlayed = true;
		player.playCardFromHandCardsAfterDelay(card, playCardStack, delay);
		updateCardIndex(card, newCardIndex);
		updateDirection(card);
		clearPermanentMessagesIfGameHasEnded();
		onPlayEventManager.fireEvent(card);
	}
	void ProxyMauMauGameData::playCardFromLocalPlayerHandCards(std::size_t indexInHandCards, CardIndex newCardIndex, int delayMs) {
		auto& localPlayer = getLocalPlayer();
		Card card = localPlayer.getCardStack().get(indexInHandCards);

		throwIfGameHasEnded();
		field_wasCardPlayed = true;
		localPlayer.playCardFromHandCardsAfterDelay(indexInHandCards, playCardStack, delayMs);
		updateCardIndex(card, newCardIndex);
		updateDirection(card);
		clearPermanentMessagesIfGameHasEnded();
		onPlayEventManager.fireEvent(card);
	}
	void ProxyMauMauGameData::playCardFromLocalPlayerTempCards(CardIndex newCardIndex, int delay) {
		auto& localPlayer = getLocalPlayer();
		auto drawnCardOrNone = localPlayer.getCardInTempStack();
		if(!drawnCardOrNone.has_value()) {
			throw std::runtime_error("LocalPlayer hasn't drawn a card in temp cards");
		}
		auto drawnCard = *drawnCardOrNone;

		throwIfGameHasEnded();
		field_wasCardPlayed = true;
		localPlayer.playCardFromTempCardStackLocal(playCardStack);
		updateCardIndex(drawnCard, newCardIndex);
		updateDirection(drawnCard);
		clearPermanentMessagesIfGameHasEnded();
		onPlayEventManager.fireEvent(drawnCard);
	}
	void ProxyMauMauGameData::updateCardIndex(Card playedCard, CardIndex newCardIndex) {
		if(newCardIndex == CardIndex::NULLINDEX) {
			this->indexForNextCard = playedCard.getCardIndex();
		} else {
			this->indexForNextCard = newCardIndex;
		}
	}
	void ProxyMauMauGameData::updateDirection(Card playedCard) {
		if(playedCard.getValue() == CHANGE_DIRECTION_VALUE && roomOptions.getOption(Options::DIRECTION_CHANGE_ON_NINE) && playerList.getAmountOfPlayers() > 2) {
			// we have to execute this code only if there are more than two players ingame, since otherways a direction change has no effect
			appendMessage("Richtungswechsel!");
			if(direction == Direction::CW) direction = Direction::CCW;
			else direction = Direction::CW;
		}
	}
	void ProxyMauMauGameData::drawInHandCardsFromCardStack(ProxyPlayer& player, Card card) {
		throwIfGameHasEnded();
		field_wasCardDrawnIntoHandCards = true;
		player.drawCardInHandCards(card, drawCardStack);
		tryRebalanceCardStacks();
	}

	void ProxyMauMauGameData::drawInHandCardsFromTempCards() {
		auto& localPlayer = getLocalPlayer();

		throwIfGameHasEnded();
		field_wasCardDrawnIntoHandCards = true;
		localPlayer.sortDrawnCardIntoHandCardsLocal();
	}

	void ProxyMauMauGameData::drawInLocalPlayerTempCards() {
		auto& localPlayer = getLocalPlayer();

		throwIfGameHasEnded();
		Card cardToDraw = getDrawCardForNextPlayer();
		localPlayer.drawSingleCardInTempCardStackLocal(cardToDraw, drawCardStack);
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

	LocalPlayer& ProxyMauMauGameData::getLocalPlayer() {
		return playerList.getLocalPlayer();
	}

	ProxyPlayer& ProxyMauMauGameData::getPlayerOnTurn() {
		return playerList.getPlayerOnTurn();
	}

	std::vector<std::shared_ptr<ProxyPlayer>> ProxyMauMauGameData::getOpponents() {
		return playerList.getOpponents();
	}

	void ProxyMauMauGameData::removeOpponentLocal(ParticipantOnClient& participant) {
		std::string participantUsername = participant.getUsername();
		appendMessage(participantUsername + u8" verlie� das Spiel.");
		clearPermanentMessagesIfGameHasEnded();

		if(hasGameEnded()) return;
		auto& player = lookupOpponent(participantUsername);

		if(playerList.isPlayerOnTurn(player)) {
			setNextPlayerOnTurnLocal();
		}

		playerList.removeOpponentLocal(player);
	}

	bool ProxyMauMauGameData::checkIfIsOpponent(std::string username) const {
		return playerList.isOpponent(username);
	}

	bool ProxyMauMauGameData::checkIfIsParticipant(std::string username) const {
		return playerList.isPlayer(username);
	}

	bool ProxyMauMauGameData::checkIfLocalPlayer(ProxyPlayer& p) const {
		return playerList.isLocalPlayer(p);
	}

	ProxyPlayer& ProxyMauMauGameData::lookupPlayer(std::string username) {
		return playerList.lookupPlayer(username);
	}
	ProxyPlayer& ProxyMauMauGameData::lookupOpponent(std::string username) {
		return playerList.lookupOpponent(username);
	}
	bool ProxyMauMauGameData::hasGameEnded() const {
		return getWinnerOrNull().has_value();
	}
	bool ProxyMauMauGameData::hasInitialCardBeenDistributed() const {
		return field_hasInitialCardsBeenDistributed;
	}
	boost::optional<ProxyPlayer&> ProxyMauMauGameData::getWinnerOrNull() const {
		const auto& allPlayers = playerList.getPlayers().getAllPlayers();

		// if there's only one player ingame, he has won the game
		if(allPlayers.size() == 1) return *allPlayers[0];

		// if the distribution of the initial cards hasn't been finished, no player can have won 
		if(! field_hasInitialCardsBeenDistributed) return boost::none;

		for(auto& player : allPlayers) {
			if(player->getCardStack().isEmptyAndNoPendingTransactions()) {
				return *player;
			}
		}
		return boost::none;
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
	void ProxyMauMauGameData::onSuccessfulMau(ProxyPlayer& player) {
		if(roomOptions.getOption(Options::HAVE_TO_MAU)) {
			player.onSuccessfulMau();
		}
	}
	void ProxyMauMauGameData::onMauPunishment(ProxyPlayer& punishedPlayer, std::vector<Card> cardsToDraw, MauPunishmentCause cause) {
		if(! roomOptions.getOption(Options::HAVE_TO_MAU)) return;

		int delay = (cause == MauPunishmentCause::NO_MAU_RECEIVED) ? PLAY_DURATION_MS : 0;
		playerHasToDrawCards(punishedPlayer, cardsToDraw, delay);

		std::string punishedUsername = punishedPlayer.getUsername();
		MauMauPunishmentMessageGenerator::appendMessage(messageQueue, punishedUsername, cause);

		if(checkIfLocalPlayer(punishedPlayer)) {
			auto& localPlayer = getLocalPlayer();
			localPlayer.onMauFailure();
		}
	}
	bool ProxyMauMauGameData::wasCardDrawnIntoHandCardsThisTurn() const {
		return field_wasCardDrawnIntoHandCards;
	}
	bool ProxyMauMauGameData::wasCardPlayedThisTurn() const {
		return field_wasCardPlayed;
	}
	void ProxyMauMauGameData::setNextPlayerOnTurnAndUpdateSkipAndDrawTwoState(Card playedCard) {
		int cardsToDrawByNextPlayer = getSizeOfCardsToDrawByCurrentPlayerDueToPlusTwo();
		int cardsToDraw = getSizeOfCardsToDrawDueToPlusTwo();
		int delayToSetNextPlayerOnTurn = getTimeToSetNextPlayerOnTurn(field_wasCardPlayed, field_wasCardDrawnIntoHandCards, cardsToDrawByNextPlayer);

		if(roomOptions.getOption(Options::PASS_SKIP)) {
			setNextPlayerOnTurnLocal();
			threadUtils_invokeIn(delayToSetNextPlayerOnTurn, [this, playedCard]() {
				if(canSkipPlayer(playedCard)) {
					auto& userOnTurn = playerList.getPlayerOnTurn();
					userOnTurn.setSkipState();
					if(checkIfLocalPlayer(userOnTurn)) {
						messageQueue.appendMessagePermanently("Du wurdest ausgelassen. Spiele eine 8 oder passe.", skipStateMessageKey);
					}
				}
			});			
		} else {
			if(canSkipPlayer(playedCard)) setNextButOnePlayerOnTurnLocal();
			else setNextPlayerOnTurnLocal();
		}
		
		if(roomOptions.getOption(Options::PASS_DRAW_TWO) && isInDrawTwoState() && playerList.isLocalPlayerOnTurn()) {
			threadUtils_invokeIn(delayToSetNextPlayerOnTurn, [this, cardsToDraw]() {
				messageQueue.appendMessagePermanently("Spiele eine 7 oder passe, um " + std::to_string(cardsToDraw) + " Karten zu ziehen.", drawTwoMessageKey);
			});
		}

		clearPermanentMessagesIfGameHasEnded();
	}
	void ProxyMauMauGameData::setNextPlayerOnTurnLocal() {
		auto& pl = playerList.getPlayers();

		auto& nextPlayer = pl.getNextPlayerOnTurn(direction);
		setOnTurnLocal(nextPlayer);
	}
	void ProxyMauMauGameData::setNextButOnePlayerOnTurnLocal() {
		auto& pl = playerList.getPlayers();

		auto& nextPlayer = pl.getNextPlayerOnTurn(direction);
		nextPlayer.startSkippedAnimation();
		auto& nextButOnePlayer = pl.getNextPlayerOnTurn(direction, nextPlayer);
		setOnTurnLocal(nextButOnePlayer);
	}
	void ProxyMauMauGameData::setOnTurnLocal(ProxyPlayer& newPlayerOnTurn) {	
		int cardsToDrawDueToPlusTwoAmount = getSizeOfCardsToDrawByCurrentPlayerDueToPlusTwo();

		// if field_wasCardPlayed would be true, the player on turn would have passed skip to the next player
		if(playerList.getPlayerOnTurn().isInSkipState() && !field_wasCardPlayed) {
			playerList.getPlayerOnTurn().startSkippedAnimation();
		}
		if(isInDrawTwoState() && !field_wasCardPlayed) {
			if(! roomOptions.getOption(Options::PASS_DRAW_TWO)) throw std::runtime_error("It isn't possible that a player has to draw cards due to a plus two, even though no (+2)-card was played when +2 cards can't be passed on.");
			playerHasToDrawCards(playerList.getPlayerOnTurn(), cardsToDrawOnPassDueToPlusTwo);
		}

		// we don't have to consider this delay if PASS_DRAW_TWO is disabled
		int delayForDrawDueToPlusTwo = PLAY_DURATION_MS + ((field_wasCardDrawnIntoHandCards) ? (DRAW_DURATION_MS + DELAY_BETWEEN_DRAW_AND_PLAY) : 0);
		int delayToSetNextPlayerOnTurn = getTimeToSetNextPlayerOnTurn(field_wasCardPlayed, field_wasCardDrawnIntoHandCards, cardsToDrawDueToPlusTwoAmount);
		int delayToFreezeAnimation = getTimeToEndCurrentTurn(field_wasCardPlayed, field_wasCardDrawnIntoHandCards);
		if(field_wasCardDrawnIntoHandCards && field_wasCardPlayed && playerList.isLocalPlayerOnTurn()) {
			// we don't have to take the time for drawing the card into consideration
			delayToFreezeAnimation = 0;
		}

		ProxyPlayer& lastUserOnTurn = playerList.getPlayerOnTurn();
		lastUserOnTurn.onEndTurn();
		onTurnEndEventManager.fireEvent(lastUserOnTurn);
		playerList.setPlayerOnTurn(newPlayerOnTurn);

		clearPermanentMessages();
		if(! field_wasCardPlayed) {
			// if field_wasCardPlayed would be true, the player on turn would have +2 to the next player
			this->cardsToDrawOnPassDueToPlusTwo.clear();
		}
		field_wasCardDrawnIntoHandCards = false;
		field_wasCardPlayed = false;
		threadUtils_invokeIn(delayToSetNextPlayerOnTurn, this, [this, &newPlayerOnTurn, &lastUserOnTurn]() {			
			lastUserOnTurn.endRemainingTimeAnimation();
			newPlayerOnTurn.onStartTurn();
			onTurnStartEventManager.fireEvent(newPlayerOnTurn);
		});
		threadUtils_invokeIn(delayToFreezeAnimation, this, [this, &lastUserOnTurn]() {
			lastUserOnTurn.freezeRemainingTimeAnimation();
		});

		if(isInDrawTwoState() && !roomOptions.getOption(Options::PASS_DRAW_TWO)) {
			playerHasToDrawCards(newPlayerOnTurn, cardsToDrawOnPassDueToPlusTwo, delayForDrawDueToPlusTwo);
			cardsToDrawOnPassDueToPlusTwo.clear();
		}
	}

	void ProxyMauMauGameData::setInitialPlayerOnTurnLocal(ProxyPlayer& player, Card nextCardOnDrawStack) {
		playerList.setPlayerOnTurn(player);
		player.onStartTurn();
		onTurnStartEventManager.fireEvent(player);

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
	
	void ProxyMauMauGameData::playerHasToDrawCards(ProxyPlayer& player, std::size_t amountOfCards, int delayMs) {
		if(amountOfCards == 0) return;

		auto cards = Card::getVectorWithCards(Card::NULLCARD, amountOfCards);
		playerHasToDrawCards(player, cards, delayMs);
	}

	void ProxyMauMauGameData::playerHasToDrawCards(ProxyPlayer& player, const std::vector<Card>& cards, int delayMs) {
		for(const Card& c : cards) {
			player.drawCardInHandCards(c, drawCardStack, delayMs);
			tryRebalanceCardStacks();

			delayMs += DRAW_MULTIPLE_DELAY_BETWEEN_CARDS_MS;
		}
	}

	void ProxyMauMauGameData::tryRebalanceCardStacks() {
		// move card from play to draw stack while there are too little cards on draw card stack and at least one card on play card stack
		while(drawCardStack.getAvailableCardsSize() <= MIN_DRAW_CARD_STACK_SIZE && playCardStack.getAvailableCardsSize() > 1) {
			drawCardStack.addFirstCardFromImmediately(Card::NULLCARD, playCardStack, REBALANCE_DURATION);
		}

		// move card from play to draw stack if there are too many cards on the play stack
		if(playCardStack.getAvailableCardsSize() > Card::MAX_CARDS) {
			while(playCardStack.getAvailableCardsSize() > 1) {
				drawCardStack.addFirstCardFromImmediately(Card::NULLCARD, playCardStack, REBALANCE_DURATION);
			}
		}

		// if there are still to little cards on draw card stack, add additional card stack
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

	EventManager<ProxyPlayer>& ProxyMauMauGameData::getTurnEndEventManager() {
		return onTurnEndEventManager;
	}

	EventManager<ProxyPlayer>& ProxyMauMauGameData::getTurnStartEventManager() {
		return onTurnStartEventManager;
	}

	EventManager<Card>& ProxyMauMauGameData::getPlayEventManager() {
		return onPlayEventManager;
	}
	
}
