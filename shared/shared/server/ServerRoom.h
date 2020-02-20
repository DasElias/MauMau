#pragma once
#include "../packet/STCPacketTransmitter.h"
#include "ServerMauMauGame.h"
#include "UsernameGenerator.h"
#include "../model/RoomCode.h"
#include "../model/RoomOptions.h"
#include "ServerGameEndHandler.h"

namespace card {
	class ServerRoom : public ServerGameEndHandler {
		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			RoomCode roomCode;
			RoomOptions roomOptions;

			std::shared_ptr<STCPacketTransmitter> packetTransmitter;
			std::vector<std::shared_ptr<ParticipantOnServer>> allParticipants;
			std::vector<std::shared_ptr<ParticipantOnServer>> participantsForNextGame;
			std::shared_ptr<ParticipantOnServer> roomLeader;

			std::unique_ptr<ServerMauMauGame> game = nullptr;

			ClientPacketListenerCallback handler_onChangeOptions;
			ClientPacketListenerCallback handler_onChangeRoomLeader;
			ClientPacketListenerCallback handler_onStartGame;
			ClientPacketListenerCallback handler_onKickPlayer;
			ClientPacketListenerCallback handler_onAiPlayerJoin;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ServerRoom(RoomCode roomCode, std::shared_ptr<STCPacketTransmitter> packetTransmitter, RoomOptions roomOptions);
			ServerRoom(const ServerRoom&) = delete;
			~ServerRoom();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			RoomCode getRoomCode() const;
			const RoomOptions& getRoomOptions() const;

			bool isGameRunning() const;
			ServerMauMauGame& getGame();
			bool checkIfLeader(const std::shared_ptr<ParticipantOnServer>& participant);
			bool checkIfLeaderByUsername(std::string username);
			bool isUsernameAvailable(std::string username);
			bool isRoomFull();
			bool checkIfParticipant(const std::shared_ptr<ParticipantOnServer>& participant);	
			bool checkIfParticipantByUsername(std::string username);
			bool checkIfParticipantInNextGame(const std::shared_ptr<ParticipantOnServer>& player);
			std::shared_ptr<ParticipantOnServer> lookupParticipantByUsername(std::string username);
			std::shared_ptr<ParticipantOnServer> getRoomLeader();
			std::vector<std::shared_ptr<ParticipantOnServer>> getParticipants();
			std::vector<std::string> getUsernamesOfParticipants();

			bool changeOptions(const std::shared_ptr<ParticipantOnServer>& sender, std::map<std::string, int> options);
			bool joinRoom(const std::shared_ptr<ParticipantOnServer>& constructedParticipant);
			bool joinRoomAiPlayer(std::string reservedUsername = "");
			bool leaveRoom(std::shared_ptr<ParticipantOnServer> participant, bool wasKickedByOtherPlayer);	// we can't pass the ptr by reference since it may be deleted after it was erased from allPlayers
			void initRoomLeaderWithoutPermissionsChecking(const std::shared_ptr<ParticipantOnServer>& newLeader);
			bool changeRoomLeader(const std::shared_ptr<ParticipantOnServer>& sender, std::string usernameOfNewLeader);
			bool startGame(const std::shared_ptr<ParticipantOnServer>& sender);

			void onGameEnd();

			optionalSuccessAnswerPacket listener_onChangeOptions(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant); 
			optionalSuccessAnswerPacket listener_onChangeRoomLeader(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant);
			optionalSuccessAnswerPacket listener_onStartGame(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant);
			optionalSuccessAnswerPacket listener_onKickPlayer(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant);
			optionalSuccessAnswerPacket listener_onAiPlayerJoin(ClientToServerPacket& p, const std::shared_ptr<ParticipantOnServer>& participant);

	};
}