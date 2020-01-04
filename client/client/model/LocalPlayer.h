#pragma once
#include "ProxyPlayer.h"
#include "ParticipantOnClient.h"
#include "PlayVerifier.h"
#include <optional>
#include <shared/packet/CTSPacketTransmitter.h>
#include <shared/model/Card.h>

namespace card {
	class LocalPlayer : public ProxyPlayer {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<ParticipantOnClient> const wrappedLocalPlayer;
			std::shared_ptr<CTSPacketTransmitter> packetTransmitter;

			PlayVerifier& playVerifier;

			bool isWaitingForColorPick_field = false;
			std::optional<Card> playedCard = std::nullopt;

			// when a card is drawn, it should appear to the user so that he can
			// play it immediately
			// this HandCardStack has always the size 0 or 1
			CardAnimator drawnCardTempStack;

			// is true when there is a card in drawCardTempStack
			// due to threading issues a scheduled animation might take
			// more than one frame to appear in drawnCardTempStack
			// since an exception is thrown when the local player tries to draw
			// a card twice, we have to use a flag
			bool wasCardDrawn_flag = false;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			LocalPlayer(std::shared_ptr<ParticipantOnClient> wrappedLocalPlayer, std::shared_ptr<CTSPacketTransmitter> packetTransmitter, PlayVerifier& playVerifier);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			// we want to override this function since we don't want to put the drawn card into the hands card
			// but rather we want to let the player choose if it should be played
			void pickLastCardFromCardStackLocal(Card mutatesTo, CardAnimator& source) override;
			void playCardLocal(Card card, CardAnimator& destination, bool isWaitingForColorPick) override;

			std::optional<Card> getPlayedCard() const;
			bool hasPlayedCard() const;

			void sortDrawnCardIntoHandCardsLocal();
			bool hasStartedToDrawCard() const;
			bool isCardInTemporaryStack() const;
			std::optional<Card> getDrawnCard() const;
			const CardAnimator& getDrawnCardAsStack() const;

			void clearWaitingForColorPickFlag();
			bool isWaitingForColorPick() const;

			void onStartTurn() override;
			void onEndTurn() override;

	};
}