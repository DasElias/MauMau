#pragma once
#include "AbstractUsernamePacket_STCPacket.h"

namespace card {
	class PlayerWantsJoinNextGame_STCPacket : public AbstractUsernamePacket_STCPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 102;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			PlayerWantsJoinNextGame_STCPacket(std::string username);
			PlayerWantsJoinNextGame_STCPacket(nlohmann::json& jsonHandle);


	};
}