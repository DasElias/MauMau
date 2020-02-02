#pragma once
#include "ProxyPlayer.h"
#include "ParticipantOnClient.h"
#include <optional>
#include <shared/model/Card.h>

namespace card {
	class LocalPlayer : public ProxyPlayer {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<ParticipantOnClient> const wrappedLocalPlayer;

			std::optional<Card> playedCard = std::nullopt;

			// when a card is drawn, it should appear to the user so that he can
			// play it immediately
			// has always the size 0 or 1
			CardAnimator drawnCardTempStack;

			// is true when there is a card in drawCardTempStack
			// due to threading issues a scheduled animation might take
			// more than one frame to appear in drawnCardTempStack
			// since an exception is thrown when the local player tries to draw
			// this card twice, we have to use a flag
			bool wasCardDrawn_flag = false;

			// is true when the local player has hit the Mau-Button
			// gets false when a negative response is received or when
			// the turn is finished
			bool wasMauDemandedThisTurn_flag = false;


		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			LocalPlayer(std::shared_ptr<ParticipantOnClient> wrappedLocalPlayer, ProxyPlayerGameInformation& gameInformation);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void drawSingleCardInTempCardStackLocal(Card mutatesTo, CardAnimator& drawCardStack);
			void playCardFromHandCards(Card card, CardAnimator& playCardStack) override;
			void playCardFromHandCardsAfterDelay(Card card, CardAnimator& playCardStack, int delayMs) override;
			void playCardFromTempCardStackLocal(CardAnimator& playCardStack);
			void sortDrawnCardIntoHandCardsLocal();

			std::optional<Card> getPlayedCard() const;
			bool hasPlayedCard() const;
			bool hasStartedToDrawCard() const;
			bool isCardInTemporaryStack() const;
			std::optional<Card> getDrawnCard() const;
			const CardAnimator& getDrawnCardAsStack() const;
			bool wasMauDemandedThisTurn() const;

			void onMauDemand();
			void onMauFailure();
			void onStartTurn() override;
			void onEndTurn() override;

	};
}