#pragma once
#include "ClientToServerPacket.h"

namespace card {
	class RoomCreationRequest_CTSPacket : public ClientToServerPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 401;

		private:
			static std::string const USERNAME_KEY;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			std::string ownUsername;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			RoomCreationRequest_CTSPacket(std::string ownUsername);
			RoomCreationRequest_CTSPacket(nlohmann::json jsonHandle);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			std::string getOwnUsername() const;

		protected:
			void addJsonProperties(nlohmann::json& jsonHandle) const override;

	};
}