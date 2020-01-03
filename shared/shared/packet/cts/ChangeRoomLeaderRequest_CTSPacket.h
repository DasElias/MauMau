#pragma once
#include "ClientToServerPacket.h"
#include <nlohmann/json.hpp>

namespace card {
	class ChangeRoomLeaderRequest_CTSPacket : public ClientToServerPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 403;

		private:
			static std::string const USERNAME_KEY;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::string username;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ChangeRoomLeaderRequest_CTSPacket(std::string usernameOfNewLeader);
			ChangeRoomLeaderRequest_CTSPacket(nlohmann::json json);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::string getUsernameOfNewLeader() const;

		protected:
			void addJsonProperties(nlohmann::json& jsonHandle) const override;
	};
}