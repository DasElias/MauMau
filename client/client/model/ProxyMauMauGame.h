#pragma once
#include <shared/packet/CTSPacketTransmitter.h>
#include <shared/model/CardIndex.h>
#include "PlayVerifier.h"
#include <shared/model/Direction.h>
#include <memory>
#include <shared/model/CardStack.h>
#include "LocalPlayer.h"
#include <shared\model\RoomOptions.h>

namespace card {
	class ProxyMauMauGame : public PlayVerifier {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static int const REBALANCE_DURATION = 750;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<CTSPacketTransmitter> packetTransmitter;
			CardAnimator drawCardStack;
			CardAnimator playCardStack;
			CardIndex indexForNextCard;
			Direction direction = Direction::CW;
			Card drawCardForNextPlayer;
			RoomOptions& roomOptions;
			ProxyPlayerGameInformation gameInformation;

			std::shared_ptr<ProxyPlayer> userOnTurn;
			std::vector<std::shared_ptr<ProxyPlayer>> allPlayers;
			std::vector<std::shared_ptr<ProxyPlayer>> opponents;
			std::shared_ptr<LocalPlayer> localPlayer;
			std::vector<std::shared_ptr<ProxyPlayer>> skippedPlayers;

			// is null, if game hasn't ended yet
			std::shared_ptr<ProxyPlayer> winner;

			ServerPacketListenerCallback handler_onOtherPlayerHasDrawnCard;
			ServerPacketListenerCallback handler_onOtherPlayerHasPlayedCard;
			ServerPacketListenerCallback handler_onLocalPlayerIsOnTurn;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ProxyMauMauGame(std::shared_ptr<CTSPacketTransmitter> packetTransmitter, std::vector<std::shared_ptr<ParticipantOnClient>> allParticipantsInclLocal, 
				std::shared_ptr<ParticipantOnClient> localParticipant, std::string usernameOnTurn,
				std::vector<int> handCards, int startCard, int nextCardOnDrawStack, RoomOptions& roomOptions);
			ProxyMauMauGame(const ProxyMauMauGame&) = delete;
			~ProxyMauMauGame();
			ProxyMauMauGame& operator=(const ProxyMauMauGame&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			bool isLocalPlayerOnTurn() const;
			bool areAllPreviousCardTransactionsCompleted() const;
			bool canPlay(std::size_t indexInLocalPlayerHandCards) const;
			bool canPlay(Card card) const override;
			bool canDraw() const;
			bool canChangeColor(Card playedCard) const;
			bool canMau() const;
			bool canPlayDrawnCard() const;
			bool canTakeDrawnCardIntoHandCards() const;
			bool canSkipPlayer(Card card) const;
			bool isWaitingForColorChoose() const;			
			std::size_t getAmountsOfCardsToDrawForNextPlayer(Card playedCard) const;


			void drawCard();
			void takeDrawnCardIntoHandCards();
			void playDrawnCard();
			void playCard(Card card);
			void playCard(std::size_t index);
			void chooseColor(CardIndex color);

			void playCardAndSetNextPlayerOnTurnLocal(std::string username, Card card, CardIndex newCardIndex, std::vector<Card> cardsToDraw, bool wasDrawedJustBefore);
			void drawCardAndSetNextPlayerOnTurnLocal(std::string username);
			// player has to draw cards after other player has played 7, for instance 
			void playerHasToDrawCards(std::shared_ptr<ProxyPlayer> player, std::size_t amountOfCards);
			void playerHasToDrawCards(std::shared_ptr<ProxyPlayer> player, const std::vector<Card>& cards);

			void setNextOrNextButOneOnTurnLocal(Card playedCard);
			void setNextPlayerOnTurnLocal();
			void setNextButOnePlayerOnTurnLocal();
			void setOnTurnLocal(std::shared_ptr<ProxyPlayer> player);
			
			// returns none if the card index for the next card is equal to the one for the last card on play stacks
			std::optional<CardIndex> getCardIndexForNextCardOrNone() const;
			const CardAnimator& getDrawStack() const;
			const CardAnimator& getPlayStack() const;
			std::shared_ptr<LocalPlayer> getLocalPlayer();
			std::vector<std::shared_ptr<ProxyPlayer>> getOpponents();
			void removeOpponentLocal(std::shared_ptr<ParticipantOnClient> player);
			bool checkIfIsOpponent(std::string username) const;
			std::shared_ptr<ProxyPlayer> lookupPlayer(std::string username);
			std::shared_ptr<ProxyPlayer> lookupOpponent(std::string username);	
			bool hasGameEnded() const;
			std::shared_ptr<ProxyPlayer> getWinnerOrNull();

		private:
			void initStartCards(const std::vector<int>& handCardNumbersOfLocalPlayer, Card cardOnPlayStack);
			void tryRebalanceCardStacks();
			void setLocalPlayerAtTheBeginOfPlayersVector();

			void sendPlayCardPacket(CardIndex newCardIndex = CardIndex::NULLINDEX);
			void updateGameEndFlag();
			void throwIfGameHasEnded();

			void listener_onOtherPlayerHasDrawnCard(Packet& p);
			void listener_onOtherPlayerHasPlayedCard(Packet& p);
			void listener_onLocalPlayerIsOnTurn(Packet& p);
	};
}