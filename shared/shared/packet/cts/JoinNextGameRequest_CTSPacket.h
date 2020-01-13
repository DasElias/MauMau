#pragma once
#include "ClientToServerPacket.h"

namespace card {
	class JoinNextGameRequest_CTSPacket : public ClientToServerPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 405;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			JoinNextGameRequest_CTSPacket();
			JoinNextGameRequest_CTSPacket(nlohmann::json& jsonHandle);

	};
}