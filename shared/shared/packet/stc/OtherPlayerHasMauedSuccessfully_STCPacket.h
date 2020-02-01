#pragma once
#include "AbstractUsernamePacket_STCPacket.h"

namespace card {
	class OtherPlayerHasMauedSuccessfully_STCPacket : public AbstractUsernamePacket_STCPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 206;


		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			OtherPlayerHasMauedSuccessfully_STCPacket(std::string username);
			OtherPlayerHasMauedSuccessfully_STCPacket(nlohmann::json& jsonHandle);

	};
}