#pragma once
#include "AbstractUsernamePacket_STCPacket.h"

namespace card {
	class PlayerWantsLeaveNextGame_STCPacket : public AbstractUsernamePacket_STCPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
	public:
		static int const PACKET_ID = 103;

	// ----------------------------------------------------------------------
	// -----------------------------CONSTRUCTORS-----------------------------
	// ----------------------------------------------------------------------
	public:
		PlayerWantsLeaveNextGame_STCPacket(std::string username);
		PlayerWantsLeaveNextGame_STCPacket(nlohmann::json jsonHandle);


	};
}