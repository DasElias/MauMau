#pragma once
#include "ClientToServerPacket.h"
#include <nlohmann/json.hpp>

namespace card {
	class KickPlayerRequest_CTSPacket : public ClientToServerPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 407;

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
			// pass by reference to prevent implicit conversion
			KickPlayerRequest_CTSPacket(const std::string& playerToKickUsername);
			KickPlayerRequest_CTSPacket(nlohmann::json& json);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::string getUsernameOfPlayerToKick() const;

		protected:
			void addJsonProperties(nlohmann::json& jsonHandle) const override;
	};
}