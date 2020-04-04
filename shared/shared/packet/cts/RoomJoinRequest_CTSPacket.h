#pragma once
#include "ClientToServerPacket.h"
#include "../../model/RoomCode.h"
#include "../../model/Avatar.h"

namespace card {
	class RoomJoinRequest_CTSPacket : public ClientToServerPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 400;

		private:
			static std::string const USERNAME_KEY;
			static std::string const ROOM_CODE_KEY;
			static std::string const AVATAR_KEY;
			static std::string const CLIENT_PROTOCOL_VERSION_KEY;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::string ownUsername;
			RoomCode roomCode;
			Avatar avatar;
			std::string clientProtocolVersion;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			RoomJoinRequest_CTSPacket(std::string ownUsername, Avatar avatar, RoomCode roomCode, std::string clientProtocolVersion);
			RoomJoinRequest_CTSPacket(nlohmann::json& jsonHandle);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::string getOwnUsername() const;
			RoomCode getRoomCode() const;
			Avatar getAvatar() const;
			std::string getClientProtocolVersion() const;

		protected:
			void addJsonProperties(nlohmann::json& jsonHandle) const override;

	};
}