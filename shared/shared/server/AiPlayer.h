#pragma once
#include "Player.h"
#include "ServerMauMauGame.h"

namespace card {
	class AiPlayer : public Player {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static float const MAU_MISS_PROBABILITY;

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
			~AiPlayer();

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
			void tryMau();
			void playCardImpl(Card c, bool wasCardJustDrawn);
			void drawCardImpl();
			CardIndex chooseCardIndex(CardIndex indexOfPlayedJack);
			bool shouldPlayDrawnCard();
			void playIfIsInSkipOrDrawTwoState();

	};
}