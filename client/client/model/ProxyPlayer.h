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
			void drawMultipleCardsInHandCardsAfterDelay(std::vector<Card> mutatesTo, CardAnimator& drawCardStack, int delayMs);
			virtual void playCardFromHandCards(Card card, CardAnimator& playCardStack);
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

			void endRemainingTimeAnimation();
			void freezeRemainingTimeAnimation();

			void onSkip();
			void onMau();
			virtual void onStartTurn();
			virtual void onEndTurn();

			bool operator==(const ProxyPlayer& p2) const;


	};
}