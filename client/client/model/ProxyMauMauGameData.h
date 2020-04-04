#pragma once
#include <shared/model/CardIndex.h>
#include <shared/model/Direction.h>
#include <memory>
#include <shared/model/CardStack.h>
#include "LocalPlayer.h"
#include <shared\model\RoomOptions.h>
#include "MessageQueue.h"
#include <shared/model/MauPunishmentCause.h>
#include <optional>
#include <functional>

namespace card {
	class ProxyMauMauGameData {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static int const REBALANCE_DURATION = 750;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			CardAnimator drawCardStack;
			CardAnimator playCardStack;
			CardIndex indexForNextCard;
			Card drawCardForNextPlayer;
			RoomOptions& roomOptions;
			Direction direction = Direction::CW;
			MessageQueue messageQueue;

			std::shared_ptr<ProxyPlayer> userOnTurn;
			std::vector<std::shared_ptr<ProxyPlayer>> allPlayers;
			std::vector<std::shared_ptr<ProxyPlayer>> opponents;
			std::shared_ptr<LocalPlayer> localPlayer;

			// when the player before has played a +2-card, the cards to draw are stored here
			std::vector<Card> cardsToDrawOnPassDueToPlusTwo;

			bool field_wasCardDrawnIntoHandCards = false;
			bool field_wasCardPlayed = false;

			bool field_hasInitialCardsBeenDistributed = false;

			std::function<void(std::shared_ptr<ProxyPlayer>)> onTurnEndCallback;

			// message that the game is in skip state
			MessageKey skipStateMessageKey;
			MessageKey drawTwoMessageKey;


		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ProxyMauMauGameData(std::vector<std::shared_ptr<ParticipantOnClient>> allParticipantsInclLocal, 
				std::shared_ptr<ParticipantOnClient> localParticipant,
				std::vector<int> handCards, int startCard, RoomOptions& roomOptions,
				std::function<void(std::shared_ptr<ProxyPlayer>)> onTurnEnd);
			ProxyMauMauGameData(const ProxyMauMauGameData&) = delete;
			ProxyMauMauGameData& operator=(const ProxyMauMauGameData&) = delete;
			~ProxyMauMauGameData();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void playCardFromHandCards(std::shared_ptr<ProxyPlayer> player, Card card, CardIndex newCardIndex = CardIndex::NULLINDEX, int delayMs = 0);
			void playCardFromLocalPlayerHandCards(std::size_t indexInHandCards, CardIndex newCardIndex = CardIndex::NULLINDEX, int delayMs = 0);
			void playCardFromLocalPlayerTempCards(CardIndex newCardIndex = CardIndex::NULLINDEX, int delayMs = 0);

			void drawInHandCardsFromCardStack(std::shared_ptr<ProxyPlayer> player, Card card);
			void drawInHandCardsFromTempCards();
			void drawInLocalPlayerTempCards();

			void onSuccessfulMau(std::shared_ptr<ProxyPlayer> player);
			void onMauPunishment(std::shared_ptr<ProxyPlayer> punishedPlayer, std::vector<Card> cardsToDraw, MauPunishmentCause cause);

			bool wasCardDrawnIntoHandCardsThisTurn() const;
			bool wasCardPlayedThisTurn() const;

			void abortTurnOnTimeExpires(const std::vector<Card>& cardsToDraw, int amountOfCardsToDrawBefore);
			// player has to draw cards after other player has played 7, for instance 
			void playerHasToDrawCards(std::shared_ptr<ProxyPlayer> player, std::size_t amountOfCards, int delayMs = 0);
			void playerHasToDrawCards(std::shared_ptr<ProxyPlayer> player, const std::vector<Card>& cards, int delayMs = 0);
			
			void setNextPlayerOnTurnAndUpdateSkipAndDrawTwoState(Card playedCard);
			void setNextPlayerOnTurnLocal();
			void setNextButOnePlayerOnTurnLocal();
			std::shared_ptr<ProxyPlayer> getNextPlayer(std::shared_ptr<ProxyPlayer> playerOnTurn);
			void setOnTurnLocal(std::shared_ptr<ProxyPlayer> player);
			void setInitialPlayerOnTurnLocal(std::shared_ptr<ProxyPlayer> player, Card nextCardOnDrawStack);

			void addCardsToDrawOnPassDueToPlusTwo(std::size_t amountOfNullcards);
			void setCardsToDrawOnPassDueToPlusTwo(std::vector<Card> cards);
			bool isInDrawTwoState() const;
			std::size_t getSizeOfCardsToDrawDueToPlusTwo() const;
			bool isLocalPlayerOnTurn() const;
			bool areAllPreviousCardTransactionsCompleted() const;
			bool isReadyToPerformLocalPlayerTurn() const;
			bool canChangeColor(Card playedCard) const;
			bool canSkipPlayer(Card card) const;
			std::size_t getAmountsOfCardsToDrawForNextPlayer(Card playedCard) const;

			
			// returns none if the card index for the next card is equal to the one for the last card on play stacks
			std::optional<CardIndex> getCardIndexForNextCardOrNone() const;
			CardIndex getCardIndexForNextCard() const;
			void setDrawCardForNextPlayer(Card c);
			Card getDrawCardForNextPlayer() const;
			const CardAnimator& getDrawStack() const;
			const CardAnimator& getPlayStack() const;
			std::shared_ptr<LocalPlayer> getLocalPlayer();
			std::shared_ptr<ProxyPlayer> getPlayerOnTurn();
			std::vector<std::shared_ptr<ProxyPlayer>> getOpponents();
			void removeOpponentLocal(std::shared_ptr<ParticipantOnClient> player);
			bool checkIfIsOpponent(std::string username) const;
			bool checkIfIsParticipant(std::string username) const;
			std::shared_ptr<ProxyPlayer> lookupPlayer(std::string username);
			std::shared_ptr<ProxyPlayer> lookupOpponent(std::string username);	
			bool hasGameEnded() const;
			bool hasInitialCardBeenDistributed() const;
			std::shared_ptr<ProxyPlayer> getWinnerOrNull() const;
			const RoomOptions& getOptions() const;
			void appendMessage(std::string content);
			const MessageQueue& getMessageQueue() const;
			MessageQueue& getMessageQueue();

		private:
			void initStartCards(const std::vector<int>& handCardNumbersOfLocalPlayer, Card cardOnPlayStack);
			void tryRebalanceCardStacks();
			void setLocalPlayerAtTheBeginOfPlayersVector();

			void updateCardIndex(Card playedCard, CardIndex newCardIndex);
			void updateDirection(Card playedCard);
			void throwIfGameHasEnded();
			void clearPermanentMessagesIfGameHasEnded();
			void clearPermanentMessages();

			void appendMauPunishmentMessage(std::string punishedUsername, MauPunishmentCause cause);

	};
}