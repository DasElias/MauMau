#pragma once
#include "ParticipantOnClient.h"
#include <shared/model/HandCardStack.h>
#include "CardAnimator.h"
#include <memory>
#include <optional>

namespace card {
	struct ProxyPlayerGameInformation {
		public:
			ProxyPlayerGameInformation() = default;
			ProxyPlayerGameInformation(const ProxyPlayerGameInformation&) = delete;

		private:
			bool wasSingleCardDrawedInHandCardsThisTurn = false;

		friend class ProxyPlayer;
	};

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
			// card is drawed and played in the same turn
			static int const DELAY_BETWEEN_DRAW_AND_PLAY = 1000;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		protected:
			CardAnimator handCardStack;

		private:
			std::shared_ptr<ParticipantOnClient> const wrappedParticipant;
			long long int unixTimeTurnStarted;
			long long int unixTimePlayerSkipped;
			ProxyPlayerGameInformation& gameInformation;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ProxyPlayer(std::shared_ptr<ParticipantOnClient> wrappedParticipant, ProxyPlayerGameInformation& gameInformation);
			ProxyPlayer(const ProxyPlayer&) = delete;
			virtual ~ProxyPlayer() = default;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void initHandCards(std::vector<Card> handCards, CardAnimator& drawCardStack, std::size_t playerIndex);
			void drawSingleCardInHandCardsLocal(Card mutatesTo, CardAnimator& drawCardStack);
			void drawMultipleCardsInHandCardsLocal(std::vector<Card> mutatesTo, CardAnimator& drawCardStack);
			void drawMultipleCardsInHandCardsAfterCardPlayTimeLocal(std::vector<Card> mutatesTo, CardAnimator& drawCardStack);
			virtual void playCardFromHandCards(Card card, CardAnimator& playCardStack, bool isWaitingForColorPick);
			virtual void playCardFromHandCardsAfterDrawTime(Card card, CardAnimator& playCardStack, bool isWaitingForColorPick);

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