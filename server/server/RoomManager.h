#pragma once
#include <vector>
#include <memory>
#include <map>
#include <shared/server/ServerRoom.h>
#include "ConnectionToClient.h"
#include "ServerPacketTransmitter.h"
#include <limits>
#include <shared/model/RoomCode.h>

namespace card {
	class RoomManager {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		private:
			static RoomCode const MIN_ROOM_CODE = 101;
			static RoomCode const MAX_ROOM_CODE = 999;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::shared_ptr<ServerPacketTransmitter> packetTransmitter;
			std::map<RoomCode, std::unique_ptr<ServerRoom>> rooms;

			ClientPacketListenerCallbackWithConnection handler_onJoinRoom;
			ClientPacketListenerCallbackWithConnection handler_onCreateRoom;
			ClientPacketListenerCallbackWithConnection handler_onKickPlayer;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			RoomManager(std::shared_ptr<ServerPacketTransmitter> packetTransmitter);
			~RoomManager();

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			void join(RoomCode roomCode, std::string username, Avatar avatar, const std::shared_ptr<ConnectionToClient>& conn);
			void createAndJoin(std::string username, Avatar avatar, std::map<std::string, int> roomOptions, const std::shared_ptr<ConnectionToClient>& conn);
			void leave(std::shared_ptr<ConnectionToClient> conn);
			[[nodiscard]] bool kick(std::string usernameOfPlayerToKick, std::shared_ptr<ConnectionToClient> requester);

		private:
			void closeRoomIfNecessary(const std::unique_ptr<ServerRoom>& room);
			void closeRoom(const std::unique_ptr<ServerRoom>& room);
			bool doesRoomExist(RoomCode roomCode);
			std::vector<std::string> getUsernamesOfOtherParticipants(const std::unique_ptr<ServerRoom>& room, std::string participantToFilter);
			std::vector<Avatar> getAvatarsOfOtherParticipants(const std::unique_ptr<ServerRoom>& room, std::string participantToFilter);
			std::vector<bool> areOtherParticipantsAiPlayers(const std::unique_ptr<ServerRoom>& room, std::string participantToFilter);

			RoomCode getNewRoomCode();
			void sendEnteringRoomSuccessReport(const std::shared_ptr<ConnectionToClient>& conn, int statusCode, std::vector<std::string> usernamesOfOtherParticipants = {}, std::vector<Avatar> avatarsOfOtherParticipants = {}, std::vector<bool> areOtherParticipantsAiPlayer = {}, std::string roomLeader = "", RoomCode roomCode = 0, std::map<std::string, int> nonDefaultOptions = {});
			optionalSuccessAnswerPacket listener_onJoinRoom(ClientToServerPacket& p, const std::shared_ptr<ConnectionToClient>& conn);
			optionalSuccessAnswerPacket listener_onCreateRoom(ClientToServerPacket& p, const std::shared_ptr<ConnectionToClient>& conn);
			optionalSuccessAnswerPacket listener_onKickPlayer(ClientToServerPacket& p, const std::shared_ptr<ConnectionToClient>& conn);
	};
}