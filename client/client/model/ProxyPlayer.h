#pragma once
#include "ParticipantOnClient.h"
#include <shared/model/HandCardStack.h>
#include "CardAnimator.h"
#include <memory>
#include <optional>

namespace card {
	class ProxyPlayer {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static long long int const NOT_ON_TURN = -1;

		protected:
			static int const PLAY_DURATION_MS = 1500;
			static int const DRAW_DURATION_MS = 1500;
			static int const DRAW_MULTIPLE_DELAY_BETWEEN_CARDS_MS = 500;
			static int const SKIP_ANIMATION_DURATION_MS = 1500;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		protected:
			CardAnimator handCardStack;

		private:
			std::shared_ptr<ParticipantOnClient> const wrappedParticipant;
			long long int unixTimeTurnStarted;
			long long int unixTimePlayerSkipped;
		
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
			void initHandCards(std::vector<Card> handCards, CardAnimator& drawCardStack, std::size_t playerIndex);
			virtual void drawCardLocal(Card mutatesTo, CardAnimator& drawCardStack);
			void drawCardsLocal(std::vector<Card> mutatesTo, CardAnimator& drawCardStack);
			void drawCardsAfterTimeCardWasPlayed(std::vector<Card> mutatesTo, CardAnimator& drawCardStack);
			virtual void playCardLocal(Card card, CardAnimator& playCardStack, bool isWaitingForColorPick);
			// this method cannot be overridden, since it will never be called on LocalPlayer
			// we need to wait until the card has been drawn only at a ProxyPlayer
			void playCardAfterTimeCardWasDrawn(Card card, CardAnimator& playCardStack, bool isWaitingForColorPick);

			const CardAnimator& getCardStack() const;
			std::string getUsername() const;
			long long int getUnixTimeTurnStarted() const;
			bool isSkipAnimationActive() const;
			float getPercentOfSkipAnimation() const;
			std::optional<float> getPercentOfSkipAnimationOrNone() const;
			bool isOnTurn() const;
			std::shared_ptr<ParticipantOnClient> getWrappedParticipiant();

			void onSkip();
			virtual void onStartTurn();
			virtual void onEndTurn();

			bool operator==(const ProxyPlayer& p2) const;


	};
}