#pragma once
#include <vector>
#include <memory>
#include <map>
#include <shared/server/ServerRoom.h>
#include "AbstractConnectionToClient.h"
#include <limits>
#include <shared/model/RoomCode.h>
#include <memory>
#include "AbstractServerPacketTransmitter.h"

namespace card {
	class RoomManager {
		public:
			struct RoomJoinReturnValue {
				ServerRoom& room;
				std::shared_ptr<ParticipantOnServer> constructedParticipant;
			};

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
			std::shared_ptr<AbstractServerPacketTransmitter> packetTransmitter;
			std::map<RoomCode, std::unique_ptr<ServerRoom>> rooms;
			std::map<std::shared_ptr<AbstractConnectionToClient>, std::pair<std::shared_ptr<ParticipantOnServer>, std::reference_wrapper<ServerRoom>>> users;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			RoomManager(std::shared_ptr<AbstractServerPacketTransmitter> abstractPacketTransmitter);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			RoomJoinReturnValue join(RoomCode roomCode, std::string username, Avatar avatar, const std::shared_ptr<AbstractConnectionToClient>& conn);
			RoomJoinReturnValue createAndJoin(std::string username, Avatar avatar, std::map<std::string, int> roomOptions, const std::shared_ptr<AbstractConnectionToClient>& conn);
			void leave(std::shared_ptr<AbstractConnectionToClient> conn);
			[[nodiscard]] bool kick(std::string usernameOfPlayerToKick, std::shared_ptr<AbstractConnectionToClient> requester);
			bool isConnectionInRoom(const std::shared_ptr<AbstractConnectionToClient>& conn) const;
			ServerRoom& getRoomToConnection(const std::shared_ptr<AbstractConnectionToClient>& conn);
			std::shared_ptr<ParticipantOnServer> getParticipantToConnection(const std::shared_ptr<AbstractConnectionToClient>& conn);
			std::shared_ptr<AbstractConnectionToClient> getConnectionToParticipant(const std::shared_ptr<ParticipantOnServer>& participant);

		private:
			void closeRoomIfNecessary(const ServerRoom& room);
			void closeRoom(const ServerRoom& room);
			bool doesRoomExist(RoomCode roomCode);
			std::vector<std::string> getUsernamesOfOtherParticipants(const std::unique_ptr<ServerRoom>& room, std::string participantToFilter);
			std::vector<Avatar> getAvatarsOfOtherParticipants(const std::unique_ptr<ServerRoom>& room, std::string participantToFilter);
			std::vector<bool> areOtherParticipantsAiPlayers(const std::unique_ptr<ServerRoom>& room, std::string participantToFilter);

			RoomCode getNewRoomCode();
			void sendEnteringRoomSuccessReport(const std::shared_ptr<AbstractConnectionToClient>& conn, int statusCode, std::vector<std::string> usernamesOfOtherParticipants = {}, std::vector<Avatar> avatarsOfOtherParticipants = {}, std::vector<bool> areOtherParticipantsAiPlayer = {}, std::string roomLeader = "", RoomCode roomCode = 0, std::map<std::string, int> nonDefaultOptions = {});
			
	};
}