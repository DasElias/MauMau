#pragma once
#include "ClientToServerPacket.h"

namespace card {
	class MauRequest_CTSPacket : public ClientToServerPacket {
		// ----------------------------------------------------------------------
		// ----------------------------STATIC-FIELDS-----------------------------
		// ----------------------------------------------------------------------
		public:
			static int const PACKET_ID = 502;

		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			MauRequest_CTSPacket();
			MauRequest_CTSPacket(nlohmann::json& jsonHandle);

	};
}