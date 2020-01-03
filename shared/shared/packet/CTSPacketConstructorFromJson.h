#pragma once
#include <string>
#include "cts/ClientToServerPacket.h"

namespace card {
	void constructAndProcessCTSPacketFromJson(std::string& json, std::function<void(ClientToServerPacket&)> callback);
}