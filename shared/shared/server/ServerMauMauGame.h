#pragma once
#include "../packet/STCPacketTransmitter.h"
#include "../packet/stc/OperationSuccessful_STCAnswerPacket.h"
#include "Player.h"
#include <vector>
#include <optional>
#include "../model/CardStack.h"
#include "../model/Direction.h"
#include "ServerGameEndHandler.h"

namespace card {
	class ServerMauMauGame {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static int const AMOUNT_OF_HAND_CARDS = 6;
			static int const CARDS_TO_DRAW_ON_TIME_EXPIRED = 2;

			static uint64_t startTurnAbortIdCounter;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<STCPacketTransmitter> packetTransmitter;
			ServerGameEndHandler& gameEndHandler;

			std::shared_ptr<Player> playerOnTurn;
			std::vector<std::shared_ptr<Player>> players;

			CardStack drawCardStack;
			CardStack playCardStack;
			CardIndex indexForNextCard;
			Direction direction = Direction::CW;

			bool wasCardDrawn_thisTurn = false;
			bool wasCardDrawn_lastTurn = false;

			bool wasCardPlayed_thisTurn = false;
			bool wasCardPlayed_lastTurn = false;

			ClientPacketListenerCallback handler_onPlayCard;
			ClientPacketListenerCallback handler_onDrawCard;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ServerMauMauGame(std::shared_ptr<STCPacketTransmitter> packetTransmitter, ServerGameEndHandler& gameEndHandler, std::vector<std::shared_ptr<ParticipantOnServer>> participants);
			~ServerMauMauGame();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			[[nodiscard]] bool playCardAndSetNextPlayerOnTurn(Player& player, Card card, CardIndex chosenIndex = CardIndex::NULLINDEX);
			[[nodiscard]] bool drawCardAndSetNextPlayerOnTurn(Player& player);
			void setNextOrNextButOneOnTurnLocal(Card playedCard);
			void setNextPlayerOnTurn();
			void setNextButOnePlayerOnTurn();
			void setPlayerOnTurn(std::shared_ptr<Player> player);

			bool canPlay(Player& player, Card card) const;
			bool canDraw(Player& player) const;
			bool canChangeColor(Card playedCard) const;
			int getAmountsOfCardsToDrawForNextPlayer(Card playedCard) const;
			bool canSkipPlayer(Card playedCard) const;
			bool canMau(Player& player) const;

			bool wasCardDrawnLastTurn() const;
			bool wasCardPlayedLastTurn() const;
			bool wasCardDrawnAndPlayedLastTurn() const;
			bool checkIfPlayerByParticipant(const std::shared_ptr<ParticipantOnServer>& participant);
			bool checkIfPlayerByUsername(std::string username);
			std::shared_ptr<Player> getPlayerOnTurn();
			std::shared_ptr<Player> lookupPlayerByParticipant(const std::shared_ptr<ParticipantOnServer>& participant);
			std::shared_ptr<Player> lookupPlayerByUsername(std::string username);
			std::shared_ptr<Player> getRandomPlayer();
			void removePlayer(std::shared_ptr<Player> player);

			const CardStack& getPlayCardStack() const;
			const CardStack& getDrawCardStack() const;

			

		private:
			void setInitialPlayerOnTurn();
			void tryRebalanceCardStacks();

			// warning: if game has ended, object will be invalidated after call
			void callGameEndFunctIfGameHasEnded();
			[[nodiscard]] bool hasPlayerWon();

			[[nodiscard]] std::vector<int> popCardsToDrawForNextPlayerFromDrawStack(int cardAmount);

			// returns false if the player tries to play a card which isn't owned by him
			[[nodiscard]] bool movePlayedCardToPlayCardStack(Player& p, Card playedCard, bool& out_wasCardDrawnAndPlayed);
			void updateColor(Card playedCard, CardIndex chosenCardIndex);

			void startTurnAbortTimer();
			void abortTurn();

			std::optional<OperationSuccessful_STCAnswerPacket> listener_onPlayCard(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant);
			std::optional<OperationSuccessful_STCAnswerPacket> listener_onDrawCard(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant);
	};
}