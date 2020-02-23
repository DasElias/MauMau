#pragma once
#include "AbstractUsernamePacket_STCPacket.h"

namespace card {
	class OtherPlayerHasLeavedRoom_STCPacket : public AbstractUsernamePacket_STCPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 101;

		private:
			static std::string const WAS_KICKED_KEY;

		// ----------------------------------------------------------------------
		// --------------------------------FIELDS--------------------------------
		// ----------------------------------------------------------------------
		private:
			bool wasKicked_field;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			OtherPlayerHasLeavedRoom_STCPacket(std::string username, bool wasKicked);
			OtherPlayerHasLeavedRoom_STCPacket(nlohmann::json& jsonHandle);

		// ----------------------------------------------------------------------
		// -------------------------------METHODS--------------------------------
		// ----------------------------------------------------------------------
		public:
			bool wasKicked() const;

		protected:
			void addJsonProperties(nlohmann::json& jsonHandle) const override;

	};
}