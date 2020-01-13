#pragma once
#include "../Packet.h"
#include <nlohmann/json.hpp>

namespace card {
	class AbstractUsernamePacket_STCPacket : public Packet {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
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
		protected:
			AbstractUsernamePacket_STCPacket(std::string username, int packetType);
			AbstractUsernamePacket_STCPacket(nlohmann::json& json, int packetType);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::string getUsername() const;

		protected:
			void addJsonProperties(nlohmann::json& jsonHandle) const override;
	};
}