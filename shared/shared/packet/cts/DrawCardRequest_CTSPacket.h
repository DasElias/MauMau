#pragma once
#include "ClientToServerPacket.h"

namespace card {
	class DrawCardRequest_CTSPacket : public ClientToServerPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 501;
			
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			DrawCardRequest_CTSPacket();
			DrawCardRequest_CTSPacket(nlohmann::json jsonHandle);

	};
}