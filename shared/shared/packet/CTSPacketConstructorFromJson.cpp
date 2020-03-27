#include "CTSPacketConstructorFromJson.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include "../utils/Logger.h"

#include "../packet/cts/ChangeOptionsRequest_CTSPacket.h"
#include "../packet/cts/ChangeRoomLeaderRequest_CTSPacket.h"
#include "../packet/cts/DrawCardRequest_CTSPacket.h"
#include "../packet/cts/GameStartRequest_CTSPacket.h"
#include "../packet/cts/MauRequest_CTSPacket.h"
#include "../packet/cts/PassRequest_CTSPacket.h"
#include "../packet/cts/PlayCardRequest_CTSPacket.h"
#include "../packet/cts/RoomCreationRequest_CTSPacket.h"
#include "../packet/cts/RoomJoinRequest_CTSPacket.h"
#include "../packet/cts/KickPlayerRequest_CTSPacket.h"
#include "../packet/cts/JoinAiPlayerRequest_CTSPacket.h"

#define PACKET_TYPE_CASE(className) case className::PACKET_ID: \
										{ \
											className pkt(jsonHandle); \
											callback(pkt); \
											break; \
										}

namespace card {
	void constructAndProcessCTSPacketFromJson(std::string& json, std::function<void(ClientToServerPacket&)> callback) {
		try {
			nlohmann::json jsonHandle = nlohmann::json::parse(json);

			int packetType = Packet::getPacketType(jsonHandle);
			switch(packetType) {
				PACKET_TYPE_CASE(ChangeOptionsRequest_CTSPacket)
					PACKET_TYPE_CASE(ChangeRoomLeaderRequest_CTSPacket)
					PACKET_TYPE_CASE(DrawCardRequest_CTSPacket)
					PACKET_TYPE_CASE(GameStartRequest_CTSPacket)
					PACKET_TYPE_CASE(MauRequest_CTSPacket)
					PACKET_TYPE_CASE(PlayCardRequest_CTSPacket)
					PACKET_TYPE_CASE(RoomCreationRequest_CTSPacket)
					PACKET_TYPE_CASE(RoomJoinRequest_CTSPacket)
					PACKET_TYPE_CASE(KickPlayerRequest_CTSPacket)
					PACKET_TYPE_CASE(JoinAiPlayerRequest_CTSPacket)
					PACKET_TYPE_CASE(PassRequest_CTSPacket)
				default:
					log(LogSeverity::ERR, "Packet with unknown packet id (" + std::to_string(packetType) + ") received!");
			}
		} catch(nlohmann::json::parse_error& pe) {
			log(LogSeverity::ERR, "Parsing error! \"" + std::string(pe.what()) + "\"");
		}

	}
}