#pragma once
#include <shared/packet/CTSPacketTransmitter.h>
#include "ProxyMauMauGameData.h"

namespace card {
	class MauMauGameAccessorFromClient {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			ProxyMauMauGameData& gameData;
			std::shared_ptr<CTSPacketTransmitter> packetTransmitter;
			
			std::optional<Card> cardToPlayAfterColorChoose = std::nullopt;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			MauMauGameAccessorFromClient(ProxyMauMauGameData& gameData, std::shared_ptr<CTSPacketTransmitter> packetTransmitter);
			MauMauGameAccessorFromClient(const MauMauGameAccessorFromClient&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			bool canMau() const;
			bool canPlay(std::size_t indexInLocalPlayerHandCards) const;
			bool canPlay(Card card) const;
			bool canDraw() const;
			bool canPlayDrawnCard() const;
			bool canTakeDrawnCardIntoHandCards() const;
			bool isWaitingForColorChoose() const;

			void mau();
			void drawCard();
			void takeDrawnCardIntoHandCards();
			void playDrawnCard();
			void playCard(std::size_t index);
			void chooseColor(CardIndex color);

			void onTurnEnd();

		private:
			void playPremarkedCardAfterColorChoose(CardIndex newCardIndex);
			void sendPlayCardPacket(CardIndex newCardIndex = CardIndex::NULLINDEX);
			


	};
}