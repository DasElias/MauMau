#pragma once
#include <string>
#include "Packet.h"

namespace card {
	void constructAndProcesSTCPacketFromJson(std::string& json, std::function<void(Packet&)> callback);
}