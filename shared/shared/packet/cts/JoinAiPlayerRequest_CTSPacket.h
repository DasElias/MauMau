#pragma once
#include "ClientToServerPacket.h"

namespace card {
	class JoinAiPlayerRequest_CTSPacket : public ClientToServerPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 408;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			JoinAiPlayerRequest_CTSPacket();
			JoinAiPlayerRequest_CTSPacket(nlohmann::json& jsonHandle);

	};
}