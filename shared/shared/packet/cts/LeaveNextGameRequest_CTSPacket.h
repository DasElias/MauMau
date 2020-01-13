#pragma once
#include "ClientToServerPacket.h"

namespace card {
	class LeaveNextGameRequest_CTSPacket : public ClientToServerPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 406;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			LeaveNextGameRequest_CTSPacket();
			LeaveNextGameRequest_CTSPacket(nlohmann::json& jsonHandle);

	};
}