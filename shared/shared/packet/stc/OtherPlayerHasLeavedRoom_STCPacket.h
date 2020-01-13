#pragma once
#include "AbstractUsernamePacket_STCPacket.h"

namespace card {
	class OtherPlayerHasLeavedRoom_STCPacket : public AbstractUsernamePacket_STCPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
	public:
		static int const PACKET_ID = 101;

	// ----------------------------------------------------------------------
	// -----------------------------CONSTRUCTORS-----------------------------
	// ----------------------------------------------------------------------
	public:
		OtherPlayerHasLeavedRoom_STCPacket(std::string username);
		OtherPlayerHasLeavedRoom_STCPacket(nlohmann::json& jsonHandle);

	};
}