#pragma once
#include "AbstractUsernamePacket_STCPacket.h"

namespace card {
	class OtherPlayerHasJoinedRoom_STCPacket : public AbstractUsernamePacket_STCPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 100;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			OtherPlayerHasJoinedRoom_STCPacket(std::string username);
			OtherPlayerHasJoinedRoom_STCPacket(nlohmann::json jsonHandle);

	};
}