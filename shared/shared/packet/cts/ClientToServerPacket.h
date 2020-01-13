#pragma once
#include "..//Packet.h"
#include <optional>

namespace card {
	class ClientToServerPacket : public Packet {
		// ----------------------------------------------------------------------
		// -----------------------------CONSTRUCTORS-----------------------------
		// ----------------------------------------------------------------------
		public:
			ClientToServerPacket(int packetType);
			ClientToServerPacket(nlohmann::json& jsonHandle, int packetType);
			ClientToServerPacket(const ClientToServerPacket&) = default;

	};
}