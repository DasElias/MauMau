#pragma once
#include <shared/packet/CTSPacketTransmitter.h>
#include "ParticipantOnClient.h"
#include "ProxyMauMauGame.h"
#include <shared/model/RoomOptions.h>

namespace card {
	class ProxyRoom {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<CTSPacketTransmitter> packetTransmitter;

			std::vector<std::shared_ptr<ParticipantOnClient>> allParticipants;
			std::vector<std::shared_ptr<ParticipantOnClient>> participantsForNextGame;
			std::shared_ptr<ParticipantOnClient> roomLeader;
			std::shared_ptr<ParticipantOnClient> localParticipant;

			std::unique_ptr<ProxyMauMauGame> game = {nullptr};
			RoomOptions options;

			bool wasGameStartRequested = false;
			bool wasParticipanceInNextGameRequested = false;
			bool wasAbsenceInNextGameRequested = false;
			bool wasRoomLeaderChangeRequested = false;

			ServerPacketListenerCallback handler_onPlayerJoinsRoom;
			ServerPacketListenerCallback handler_onPlayerLeavesRoom;
			ServerPacketListenerCallback handler_onPlayerJoinsNextRound;
			ServerPacketListenerCallback handler_onPlayerLeavesNextRound;
			ServerPacketListenerCallback handler_onOptionsWereChanged;
			ServerPacketListenerCallback handler_onRoomLeaderChange;
			ServerPacketListenerCallback handler_onGameStart;


		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ProxyRoom(std::shared_ptr<CTSPacketTransmitter> packetTransmitter, std::vector<std::string> opponentsUsernames, std::string localPlayerUsername, std::string usernameOfLeader, std::map<std::string, int> options);
			~ProxyRoom();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			bool isGameRunning() const;
			ProxyMauMauGame& getGame();
			bool isLocalUserLeader() const;
			bool isLocalUserParticipantInNextGame() const;
			bool isUsernameAvailable(std::string username) const;
			bool isParticipantInNextGame(std::string username) const;
			bool isParticipantInNextGame(const std::shared_ptr<ParticipantOnClient>& participant);
			std::shared_ptr<ParticipantOnClient> lookupParticipant(std::string username);

			bool canStartGame();
			void requestGameStart();
			bool canJoinQueueForNextGame();
			void requestJoinQueueForNextGame();
			bool canLeaveQueueForNextGame();
			void requestLeaveQueueForNextGame();
			void requestRoomLeaderChange(std::string newRoomLeaderUsername);
			bool canBeRoomLeader(std::string username);

			void joinPlayerLocal(std::string username);
			void kickPlayerLocal(std::string username);
			void joinQueueForNextGameLocal(std::string username);
			void leaveQueueForNextGameLocal(std::string username);
			void changeRoomLeaderLocal(std::string newLeaderUsername);
			void startGameLocal(std::vector<std::string> usernamesOfAllPlayers, std::string usernameOnTurn, std::vector<int> handCards, int startCard, int nextCardOnDrawStack);

		private:
			void listener_onPlayerJoinsRoom(Packet& p);
			void listener_onPlayerLeavesRoom(Packet& p);
			void listener_onPlayerJoinsNextGame(Packet& p);
			void listener_onPlayerLeavesNextGame(Packet& p);
			void listener_onOptionsWereChanged(Packet& p);
			void listener_onRoomLeaderChange(Packet& p);
			void listener_onGameStart(Packet& p);
			

	};
}