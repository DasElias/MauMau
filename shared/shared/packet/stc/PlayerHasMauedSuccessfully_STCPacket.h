#pragma once
#include "AbstractUsernamePacket_STCPacket.h"

namespace card {
	class PlayerHasMauedSuccessfully_STCPacket : public AbstractUsernamePacket_STCPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 206;


		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			PlayerHasMauedSuccessfully_STCPacket(std::string username);
			PlayerHasMauedSuccessfully_STCPacket(nlohmann::json& jsonHandle);

	};
}