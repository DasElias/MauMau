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
			static long long int const ANIMATION_NOT_FREEZED = -1;

			static int const SKIP_ANIMATION_DURATION_MS;
			static int const MAU_ANIMATION_DURATION_MS;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		protected:
			CardAnimator handCardStack;

		private:
			std::shared_ptr<ParticipantOnClient> const wrappedParticipant;
			long long int unixTimeOnTurnAnimationStarted;
			long long int unixTimeOnTurnAnimationFreezed;
			long long int unixTimePlayerSkipped;
			long long int unixTimePlayerMaued;
			bool isInSkipState_field;
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
			
			// a method to draw multiple cards is not offered, since the draw card stack has to be rebalanced after every drawn card
			void drawCardInHandCardsDueToUserAction(Card mutatesTo, CardAnimator& drawCardStack, int delayMs = 0);	// when a player chooses to draw a card
			void drawCardInHandCardsAsPunishment(Card mutatesTo, CardAnimator& drawCardStack, int delayMs = 0);	// for example, after a +2-card

			virtual void playCardFromHandCardsAfterDelay(Card card, CardAnimator& playCardStack, int delayMs);

			const CardAnimator& getCardStack() const;
			std::string getUsername() const;
			Avatar getAvatar() const;
			bool isSkipAnimationActive() const;
			float getPercentOfSkipAnimation() const;
			std::optional<float> getPercentOfSkipAnimationOrNone() const;
			bool isMauAnimationActive() const;
			float getPercentOfMauAnimation() const;
			std::optional<float> getPercentOfMauAnimationOrNone() const;
			bool hasTimeExpired() const;
			bool isRemainingTimeAnimationActive() const;
			std::optional<float> getPercentOfRemainingTime() const;
			std::shared_ptr<ParticipantOnClient> getWrappedParticipiant();
			bool isInSkipState() const;

			void endRemainingTimeAnimation();
			void freezeRemainingTimeAnimation();

			void setSkipState();
			void startSkippedAnimation();
			void onSuccessfulMau();
			virtual void onStartTurn();
			virtual void onEndTurn();

			bool operator==(const ProxyPlayer& p2) const;


	};
}