#pragma once
#include "AbstractUsernamePacket_STCPacket.h"

namespace card {
	class GameHasStartedButNotParticipant_STCPacket : public Packet {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 108;


		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			GameHasStartedButNotParticipant_STCPacket();
			GameHasStartedButNotParticipant_STCPacket(nlohmann::json& jsonHandle);

	};
}