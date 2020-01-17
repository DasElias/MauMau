#pragma once
#include "../packet/STCPacketTransmitter.h"
#include "../packet/stc/OperationSuccessful_STCAnswerPacket.h"
#include "Player.h"
#include <vector>
#include <optional>
#include "../model/CardStack.h"
#include "../model/Direction.h"

namespace card {
	class ServerMauMauGame {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:	
			static int const MAX_DURATION_OF_TURN_MS = 30 * 1000;

		private:
			static int const AMOUNT_OF_HAND_CARDS = 6;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<STCPacketTransmitter> packetTransmitter;

			std::shared_ptr<Player> playerOnTurn;
			std::vector<std::shared_ptr<Player>> players;

			CardStack drawCardStack;
			CardStack playCardStack;
			CardIndex indexForNextCard;
			Direction direction = Direction::CW;

			bool wasCardDrawnAndPlayed_thisTurn = false;
			bool wasCardDrawnAndPlayed_lastTurn = false;

			ClientPacketListenerCallback handler_onPlayCard;
			ClientPacketListenerCallback handler_onDrawCard;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ServerMauMauGame(std::shared_ptr<STCPacketTransmitter> packetTransmitter, std::vector<std::shared_ptr<ParticipantOnServer>> participants);
			~ServerMauMauGame();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			bool canPlay(Player& player, Card card) const;
			bool canDraw(Player& player) const;
			bool canChangeColor(Card playedCard) const;
			int getAmountsOfCardsToDrawForNextPlayer(Card playedCard) const;
			bool canSkipPlayer(Card playedCard) const;
			bool canMau(Player& player) const;

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

			void setPlayerOnTurn(std::shared_ptr<Player> player);
			void setNextPlayerOnTurn();
			void setNextButOnePlayerOnTurn();
			bool playCardAndSetNextPlayerOnTurn(Player& player, Card card, CardIndex chosenIndex = CardIndex::NULLINDEX);
			bool drawCardAndSetNextPlayerOnTurn(Player& player);

		private:
			void setInitialPlayerOnTurn();
			void tryRebalanceCardStacks();

			std::optional<OperationSuccessful_STCAnswerPacket> listener_onPlayCard(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant);
			std::optional<OperationSuccessful_STCAnswerPacket> listener_onDrawCard(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant);
	};
}