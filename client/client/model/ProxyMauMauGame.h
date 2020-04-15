#pragma once
#include "ProxyMauMauGameData.h"
#include "MauMauGameAccessorFromClient.h"
#include "MauMauGameAccessorFromServer.h"
#include "MauMauTutorialWrapper.h"

namespace card {
	class ProxyMauMauGame {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			ProxyMauMauGameData gameData;
			MauMauGameAccessorFromClient accessorFromClient;
			MauMauGameAccessorFromServer accessorFromServer;
			MauMauTutorialWrapper tutorialWrapper;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ProxyMauMauGame(std::shared_ptr<CTSPacketTransmitter> packetTransmitter, std::vector<std::shared_ptr<ParticipantOnClient>> allParticipantsInclLocal,
								std::shared_ptr<ParticipantOnClient> localParticipant, 
								std::vector<int> handCards, int startCard, RoomOptions& roomOptions);
			ProxyMauMauGame(const ProxyMauMauGame&) = delete;
			ProxyMauMauGame& operator=(const ProxyMauMauGame&) = delete;

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			const ProxyMauMauGameData& getGameData() const;
			MauMauGameAccessorFromClient& getAccessorFromClient();

			const CardAnimator& getDrawStack() const;
			const CardAnimator& getPlayStack() const;
			LocalPlayer& getLocalPlayer();
			std::vector<std::shared_ptr<ProxyPlayer>> getOpponents();
			void removeOpponentLocal(ParticipantOnClient& player);
			bool checkIfIsOpponent(std::string username) const;
			bool hasGameEnded() const;
			bool hasInitialCardBeenDistributed() const;
			boost::optional<ProxyPlayer&> getWinnerOrNull();
			MessageQueue& getMessageQueue();
			void clearPermanentMessages();
	};
}