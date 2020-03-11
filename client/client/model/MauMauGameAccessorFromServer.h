#pragma once
#include <shared/packet/CTSPacketTransmitter.h>
#include "ProxyMauMauGameData.h"
namespace card {
	class MauMauGameAccessorFromServer {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			ProxyMauMauGameData& gameData;
			std::shared_ptr<CTSPacketTransmitter> packetTransmitter;
		
			ServerPacketListenerCallback handler_onOtherPlayerHasDrawnCard;
			ServerPacketListenerCallback handler_onOtherPlayerHasPlayedCard;
			ServerPacketListenerCallback handler_onOtherPlayerHasPassed;
			ServerPacketListenerCallback handler_onLocalPlayerIsOnTurn;
			ServerPacketListenerCallback handler_onTimeExpires;
			ServerPacketListenerCallback handler_onMauPunishment;
			ServerPacketListenerCallback handler_onOtherPlayerHasSuccessfullyMaued;
			ServerPacketListenerCallback handler_initialPlayerIsOnTurn;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			MauMauGameAccessorFromServer(ProxyMauMauGameData& gameData, std::shared_ptr<CTSPacketTransmitter> packetTransmitter);
			MauMauGameAccessorFromServer(const MauMauGameAccessorFromServer&) = delete;
			~MauMauGameAccessorFromServer();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void playCardAndSetNextPlayerOnTurnLocal(std::string username, Card card, CardIndex newCardIndex, std::vector<Card> cardsToDraw, bool wasDrawedJustBefore);
			void drawCardAndSetNextPlayerOnTurnLocal(std::string username);

		private:
			void listener_onOtherPlayerHasDrawnCard(Packet& p);
			void listener_onOtherPlayerHasPlayedCard(Packet& p);
			void listener_onOtherPlayerHasPassed(Packet& p);
			void listener_onLocalPlayerIsOnTurn(Packet& p);
			void listener_onTimeExpires(Packet& p);
			void listener_onMauPunishment(Packet& p);
			void listener_onOtherPlayerHasSuccessfullyMaued(Packet& p);
			void listener_initialPlayerIsOnTurn(Packet& p);

	};
}