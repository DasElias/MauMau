#pragma once
#include "ClientToServerPacket.h"

namespace card {
	class GameStartRequest_CTSPacket : public ClientToServerPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 404;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			GameStartRequest_CTSPacket();
			GameStartRequest_CTSPacket(nlohmann::json& jsonHandle);

	};
}