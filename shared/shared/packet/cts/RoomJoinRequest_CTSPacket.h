#pragma once
#include "ClientToServerPacket.h"
#include "../../model/RoomCode.h"

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

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::string ownUsername;
			RoomCode roomCode;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			RoomJoinRequest_CTSPacket(std::string ownUsername, RoomCode roomCode);
			RoomJoinRequest_CTSPacket(nlohmann::json jsonHandle);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::string getOwnUsername() const;
			RoomCode getRoomCode() const;

		protected:
			void addJsonProperties(nlohmann::json& jsonHandle) const override;

	};
}