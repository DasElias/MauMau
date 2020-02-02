#include "STCPacketConstructorFromJson.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include "../utils/Logger.h"

#include "../packet/stc/OtherPlayerHasJoinedRoom_STCPacket.h"
#include "../packet/stc/OtherPlayerHasLeavedRoom_STCPacket.h"
#include "../packet/stc/PlayerWantsJoinNextGame_STCPacket.h"
#include "../packet/stc/PlayerWantsLeaveNextGame_STCPacket.h"
#include "../packet/stc/RoomLeaderHasChanged_STCPacket.h"
#include "../packet/stc/GameHasBeenStarted_STCPacket.h"
#include "../packet/stc/OptionsWereChanged_STCPacket.h"
#include "../packet/stc/EnteringRoomSuccessReport_STCAnswerPacket.h"
#include "../packet/stc/GameHasStartedButNotParticipant_STCPacket.h"
#include "../packet/stc/OtherPlayerHasDrawnCards_STCPacket.h"
#include "../packet/stc/OtherPlayerHasPlayedCard_STCPacket.h"
#include "../packet/stc/OperationSuccessful_STCAnswerPacket.h"
#include "../packet/stc/LocalPlayerIsOnTurn_STCPacket.h"
#include "../packet/stc/TurnWasAborted_STCPacket.h"
#include "../packet/stc/MauPunishment_STCPacket.h"
#include "../packet/stc/PlayerHasMauedSuccessfully_STCPacket.h"

#define PACKET_TYPE_CASE(className) case className::PACKET_ID: \
										{ \
											className pkt(jsonHandle); \
											callback(pkt); \
											break; \
										}

namespace card {
	void constructAndProcesSTCPacketFromJson(std::string& json, std::function<void(Packet&)> callback) {
		nlohmann::json jsonHandle = nlohmann::json::parse(json);
		int packetType = Packet::getPacketType(jsonHandle);

		switch(packetType) {
			PACKET_TYPE_CASE(OtherPlayerHasJoinedRoom_STCPacket)
			PACKET_TYPE_CASE(OtherPlayerHasLeavedRoom_STCPacket)
			PACKET_TYPE_CASE(PlayerWantsJoinNextGame_STCPacket)
			PACKET_TYPE_CASE(PlayerWantsLeaveNextGame_STCPacket)
			PACKET_TYPE_CASE(RoomLeaderHasChanged_STCPacket)
			PACKET_TYPE_CASE(GameHasBeenStarted_STCPacket)
			PACKET_TYPE_CASE(OptionsWereChanged_STCPacket)
			PACKET_TYPE_CASE(EnteringRoomSuccessReport_STCAnswerPacket)
			PACKET_TYPE_CASE(GameHasStartedButNotParticipant_STCPacket)
			PACKET_TYPE_CASE(OtherPlayerHasDrawnCards_STCPacket)
			PACKET_TYPE_CASE(OtherPlayerHasPlayedCard_STCPacket)
			PACKET_TYPE_CASE(OperationSuccessful_STCAnswerPacket)
			PACKET_TYPE_CASE(LocalPlayerIsOnTurn_STCPacket)
			PACKET_TYPE_CASE(TurnWasAborted_STCPacket)
			PACKET_TYPE_CASE(MauPunishment_STCPacket)
			PACKET_TYPE_CASE(PlayerHasMauedSuccessfully_STCPacket)
			default:
				log(LogSeverity::ERR, "Packet with unknown packet id (" + std::to_string(packetType) + ") received!");
		}
	}
}