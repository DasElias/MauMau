#pragma once
#include "ClientToServerPacket.h"

namespace card {
	class PassRequest_CTSPacket : public ClientToServerPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 503;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			PassRequest_CTSPacket();
			PassRequest_CTSPacket(nlohmann::json& jsonHandle);

	};
}