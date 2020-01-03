#pragma once
#include "ParticipantOnClient.h"
#include <shared/model/HandCardStack.h>
#include "CardAnimator.h"
#include <memory>

namespace card {
	class ProxyPlayer {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static long long int const NOT_ON_TURN = -1;

		protected:
			static int const PLAY_DURATION_MS = 2000;
			static int const DRAW_DURATION_MS = 2000;
			static int const DRAW_MULTIPLE_DELAY_BETWEEN_CARDS_MS = 1000;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		protected:
			CardAnimator handCardStack;

		private:
			std::shared_ptr<ParticipantOnClient> const wrappedParticipant;
			long long int unixTimeTurnStarted;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ProxyPlayer(std::shared_ptr<ParticipantOnClient> wrappedParticipant, std::vector<Card> handCards = {});
			ProxyPlayer(const ProxyPlayer&) = delete;
			virtual ~ProxyPlayer() = default;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void addHandCardFromPlainLocal(Card c);
			void addHandCardsFromPlainLocal(std::vector<Card> cards);
			virtual void pickLastCardFromCardStackLocal(Card mutatesTo, CardAnimator& drawCardStack);
			void pickLastCardsFromCardStackLocal(std::vector<Card> mutatesTo, CardAnimator& drawCardStack);
			virtual void playCardLocal(Card card, CardAnimator& playCardStack, bool isWaitingForColorPick);

			void removeHandCardLocal(Card c);
			const CardAnimator& getCardStack() const;
			std::string getUsername() const;
			long long int getUnixTimeTurnStarted() const;
			bool isOnTurn() const;
			std::shared_ptr<ParticipantOnClient> getWrappedParticipiant();

			virtual void onStartTurn();
			virtual void onEndTurn();

			bool operator==(const ProxyPlayer& p2) const;


	};
}