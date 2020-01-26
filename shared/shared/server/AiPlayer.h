#pragma once
#include "Player.h"
#include "ServerMauMauGame.h"

namespace card {
	class AiPlayer : public Player {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			ServerMauMauGame& game;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			AiPlayer(std::shared_ptr<ParticipantOnServer> wrappedParticipant, ServerMauMauGame& game, std::vector<Card> handCards = {});

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void onStartTurn() override;
			void performTurn();

		private:
			// return true if a card was played
			bool tryPlayCard();
			std::vector<Card> getPlayableCards();
			void playCardImpl(Card c);
			void drawCardImpl();
			CardIndex chooseCardIndex();
			bool shouldPlayDrawnCard();

	};
}