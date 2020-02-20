#pragma once
#include "../Packet.h"
#include <vector>
#include "../../model/RoomCode.h"
#include "../../model/Avatar.h"

namespace card {
	class EnteringRoomSuccessReport_STCAnswerPacket : public Packet {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 301;

			static int const SUCCESS_STATUS = 0;
			static int const UNKNOWN_ERROR_STATUS = 1;
			static int const ROOM_NOT_FOUND_STATUS = 2;
			static int const USERNAME_TAKEN_STATUS = 3;
			static int const ROOM_FULL_STATUS = 4;

		private:
			static std::string const STATUS_CODE_KEY;
			static std::string const USERNAMES_OF_OTHER_PARTICIPANTS_KEY;
			static std::string const AVATARS_OF_OTHER_PARTICIPANTS_KEY;
			static std::string const ARE_OTHER_PARTICIPANTS_AI_PLAYERS_KEY;
			static std::string const ROOM_LEADER_KEY;
			static std::string const ROOM_CODE_KEY;
			static std::string const OPTIONS_KEY;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			int statusCode;
			std::vector<std::string> usernamesOfOtherParticipants;
			std::vector<Avatar> avatarsOfOtherParticipants;
			std::vector<bool> areOtherParticipantsAiPlayers_field;
			std::string roomLeader;
			RoomCode roomCode;
			std::map<std::string, int> options;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			EnteringRoomSuccessReport_STCAnswerPacket(int statusCode, std::vector<std::string> usernamesOfOtherParticipants, std::vector<Avatar> avatarsOfOtherParticipants, std::vector<bool> areOtherParticipantsAiPlayers, std::string roomLeader, RoomCode roomCode, std::map<std::string, int> options);
			EnteringRoomSuccessReport_STCAnswerPacket(const EnteringRoomSuccessReport_STCAnswerPacket&) = default;
			EnteringRoomSuccessReport_STCAnswerPacket(nlohmann::json& jsonHandle);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			int getStatusCode() const;
			std::vector<std::string> getUsernamesOfOtherParticipants() const;
			std::vector<Avatar> getAvatarsOfOtherParticipants() const;
			std::vector<bool> areOtherParticipantsAiPlayers() const;
			std::string getRoomLeader() const;
			RoomCode getRoomCode() const;
			std::map<std::string, int> getOptions() const;

			void addJsonProperties(nlohmann::json& jsonHandle) const override;

	};
}