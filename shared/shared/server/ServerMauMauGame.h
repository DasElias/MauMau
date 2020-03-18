#pragma once
#include "../packet/STCPacketTransmitter.h"
#include "../packet/stc/OperationSuccessful_STCAnswerPacket.h"
#include "Player.h"
#include <vector>
#include <optional>
#include "../model/CardStack.h"
#include "../model/Direction.h"
#include "ServerGameEndHandler.h"
#include "../model/MauPunishmentCause.h"
#include "../model/RoomOptions.h"

namespace card {
	class ServerMauMauGame {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static int const CARDS_TO_DRAW_ON_TIME_EXPIRED = 2;
			static int const CARDS_TO_DRAW_MAU_PUNISHMENT = 2;
			static int const MAX_CARDS_INGAME = 3 * Card::MAX_CARDS;

			static uint64_t startTurnAbortIdCounter;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<STCPacketTransmitter> packetTransmitter;
			ServerGameEndHandler& gameEndHandler;
			RoomOptions& roomOptions;

			std::shared_ptr<Player> playerOnTurn;
			std::vector<std::shared_ptr<Player>> players;

			CardStack drawCardStack;
			CardStack playCardStack;
			CardIndex indexForNextCard;
			Direction direction = Direction::CW;
			int cardsIngameSum = 0;

			bool wasCardDrawn_thisTurn = false;
			bool wasCardDrawn_lastTurn = false;

			bool wasCardPlayed_thisTurn = false;
			bool wasCardPlayed_lastTurn = false;

			bool wasMauedCorrectly_thisTurn = false;

			bool isInSkipState_field = false;
			int amountOfDrawedCardsDueToPlusTwoLastTurn = 0;
			std::vector<int> cardsToDrawDueToPlusTwo;

			ClientPacketListenerCallback handler_onPlayCard;
			ClientPacketListenerCallback handler_onDrawCard;
			ClientPacketListenerCallback handler_onMau;
			ClientPacketListenerCallback handler_onPass;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ServerMauMauGame(std::shared_ptr<STCPacketTransmitter> packetTransmitter, ServerGameEndHandler& gameEndHandler, std::vector<std::shared_ptr<ParticipantOnServer>> participants, RoomOptions& options);
			~ServerMauMauGame();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void mau(Player& player);
			[[nodiscard]] bool playCardAndSetNextPlayerOnTurn(Player& player, Card card, bool wasCardJustDrawn, CardIndex chosenIndex = CardIndex::NULLINDEX);
			[[nodiscard]] bool drawCardAndSetNextPlayerOnTurn(Player& player);
			[[nodiscard]] bool pass(Player& player);

			void setNextPlayerOnTurnAndUpdateSkipState(Card playedCard);
			void setNextPlayerOnTurn();
			std::shared_ptr<Player> getNextPlayer(std::shared_ptr<Player> playerOnTurn);
			void setNextButOnePlayerOnTurn();
			void setPlayerOnTurn(std::shared_ptr<Player> player);

			bool canPlay(Player& player, Card card) const;
			bool canDraw(Player& player) const;
			bool canChangeColor(Card playedCard) const;
			int getAmountsOfCardsToDrawForNextPlayer(Card playedCard) const;
			bool canSkipPlayer(Card playedCard) const;
			bool canMau(Player& player) const;
			bool canPass(Player& player) const;

			bool isInSkipState() const;
			bool isInDrawTwoState() const;
			bool wasCardDrawnLastTurn() const;
			bool wasCardPlayedLastTurn() const;
			bool wasCardDrawnAndPlayedLastTurn() const;
			int getAmountOfDrawedCardsDueToPlusTwoLastTurn() const;
			bool checkIfPlayerByParticipant(const std::shared_ptr<ParticipantOnServer>& participant);
			bool checkIfPlayerByUsername(std::string username);
			bool checkIfOnTurn(Player& p) const;
			std::shared_ptr<Player> getPlayerOnTurn();
			std::shared_ptr<Player> lookupPlayerByParticipant(const std::shared_ptr<ParticipantOnServer>& participant);
			std::shared_ptr<Player> lookupPlayerByUsername(std::string username);
			std::shared_ptr<Player> getRandomPlayer();
			void removePlayer(std::shared_ptr<Player> player);
			const RoomOptions& getOptions() const;
			std::size_t getAmountOfParticipants() const;

			const CardStack& getPlayCardStack() const;
			const CardStack& getDrawCardStack() const;

			

		private:
			void setInitialPlayerOnTurn();
			void tryRebalanceCardStacks();
			void addCardDeckToDrawStack();

			// warning: if game has ended, object will be invalidated after call
			void callGameEndFunctIfGameHasEnded();
			[[nodiscard]] bool hasPlayerWon();

			[[nodiscard]] std::vector<int> popCardsFromDrawStack(int cardAmount);

			// returns false if the player tries to play a card which isn't owned by him
			[[nodiscard]] bool movePlayedCardToPlayCardStack(Player& p, Card playedCard, bool wasCardJustDrawn);
			void updateColor(Card playedCard, CardIndex chosenCardIndex);
			void updateDirection(Card playedCard);

			// checks, if the player on turn has only one hand card´and if not both wasCardPlayed_thisTurn and wasCardDrawn_thisTurn are true,
			// since the player can't mau if drawn the card just drawn
			void checkForMauIfNeeded();
			void sendMauPunishmentPacket(Player& responsiblePlayer, MauPunishmentCause cause);
			void sendSuccessfulMauPacket(Player& responsiblePlayer);

			void startTurnAbortTimer();
			void abortTurn();

			std::optional<OperationSuccessful_STCAnswerPacket> listener_onPlayCard(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant);
			std::optional<OperationSuccessful_STCAnswerPacket> listener_onDrawCard(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant);
			std::optional<OperationSuccessful_STCAnswerPacket> listener_onMau(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant);
			std::optional<OperationSuccessful_STCAnswerPacket> listener_onPass(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant);


	};
}