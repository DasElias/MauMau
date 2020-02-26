#pragma once
#include <shared/packet/CTSPacketTransmitter.h>
#include "ParticipantOnClient.h"
#include "ProxyMauMauGame.h"
#include <shared/model/RoomOptions.h>
#include <shared/model/RoomCode.h>
#include "AbstractRoomLeaveHandler.h"

namespace card {
	class ProxyRoom {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<CTSPacketTransmitter> packetTransmitter;
			AbstractRoomLeaveHandler& roomLeaveHandler;
			RoomCode roomCode;

			std::vector<std::shared_ptr<ParticipantOnClient>> allParticipants;
			std::shared_ptr<ParticipantOnClient> roomLeader;
			std::shared_ptr<ParticipantOnClient> localParticipant;

			std::unique_ptr<ProxyMauMauGame> game = {nullptr};
			RoomOptions options;

			bool isWaitingForResponse_field = false;

			ServerPacketListenerCallback handler_onPlayerJoinsRoom;
			ServerPacketListenerCallback handler_onOtherPlayerLeavesRoom;
			ServerPacketListenerCallback handler_onOptionsWereChanged;
			ServerPacketListenerCallback handler_onRoomLeaderChange;
			ServerPacketListenerCallback handler_onGameStart;


		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ProxyRoom(std::shared_ptr<CTSPacketTransmitter> packetTransmitter, AbstractRoomLeaveHandler& roomLeaveHandler, RoomCode roomCode,
					  std::vector<std::string> opponentUsernames, std::vector<Avatar> opponentAvatars, std::vector<bool> areOpponentsAiPlayers, 
					  std::string localPlayerUsername, Avatar localPlayerAvatar, std::string usernameOfLeader, RoomOptions options);
			~ProxyRoom();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			bool isGameRunning() const;
			ProxyMauMauGame& getGame();
			bool isWaitingForResponse() const;
			bool isLocalUserLeader() const;
			bool isLeader(std::string username);
			bool isLeader(std::shared_ptr<ParticipantOnClient> user);
			bool isLocalUser(std::shared_ptr<ParticipantOnClient> user);
			bool isUsernameAvailable(std::string username) const;
			std::shared_ptr<ParticipantOnClient> lookupParticipant(std::string username);
			const std::vector<std::shared_ptr<ParticipantOnClient>>& getAllParticipants();
			std::shared_ptr<ParticipantOnClient> getLocalParticipant();
			std::shared_ptr<ParticipantOnClient> getRoomLeader();
			const RoomOptions& getOptions() const;
			RoomCode getRoomCode() const;

			bool canStartGame();
			void requestGameStart();
			void requestRoomLeaderChange(std::shared_ptr<ParticipantOnClient> newRoomLeader);
			bool canBeRoomLeader(std::shared_ptr<ParticipantOnClient> user);
			bool canBeKicked(std::shared_ptr<ParticipantOnClient> playerToKick);
			void requestKickPlayer(std::shared_ptr<ParticipantOnClient> playerToKick);
			void requestOptionChange(RoomOptions options);
			bool canAiPlayerJoin();
			void requestAiPlayerJoin();
			void requestLeave();

			void joinPlayerLocal(std::string username, Avatar avatar, bool isAiPlayer);
			void kickOtherPlayerLocal(std::string username);
			void changeOptionsLocal(RoomOptions options);
			void changeRoomLeaderLocal(std::string newLeaderUsername);
			void startGameLocal(std::vector<std::string> usernamesOfAllPlayers, std::vector<int> handCards, int startCard);

		private:
			void listener_onPlayerJoinsRoom(Packet& p);
			void listener_onOtherPlayerLeavesRoom(Packet& p);
			void listener_onOptionsWereChanged(Packet& p);
			void listener_onRoomLeaderChange(Packet& p);
			void listener_onGameStart(Packet& p);
			

	};
}