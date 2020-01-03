#pragma once
#include "AbstractUsernamePacket_STCPacket.h"

namespace card {
	class RoomLeaderHasChanged_STCPacket : public AbstractUsernamePacket_STCPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 104;


		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			RoomLeaderHasChanged_STCPacket(std::string username);
			RoomLeaderHasChanged_STCPacket(nlohmann::json jsonHandle);

	};
}