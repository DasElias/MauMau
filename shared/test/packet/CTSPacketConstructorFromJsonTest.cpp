#include <catch/catch.hpp>
#include <packet/CTSPacketConstructorFromJson.h>
#include <packet/cts/RoomJoinRequest_CTSPacket.h>
#include <optional>
#include <boost/optional.hpp>

using namespace card;

auto const requireNotExecuted = [](ClientToServerPacket&) {
	REQUIRE(false);
};

TEST_CASE("CTSPacketConstructorFromJson", "[CTSPacketConstructorFromJson]") {
	SECTION("call with invalid json #1") {
		std::string json = "";
		constructAndProcessCTSPacketFromJson(json, requireNotExecuted);
	}
	SECTION("call with invalid json #2") {
		std::string json = "{pType: \"HALLO\"}";
		constructAndProcessCTSPacketFromJson(json, requireNotExecuted);
	}
	SECTION("call with invalid json #2") {
		std::string json = "{pType: \"401\"}";
		constructAndProcessCTSPacketFromJson(json, requireNotExecuted);
	}
	SECTION("call with correct json") {
		std::string const username = "Ha";
		RoomCode const roomCode = 212;
		Avatar const avatar = 1;
		std::string const clientProtocolVersion = "A";
		RoomJoinRequest_CTSPacket roomJoinRequestPacket(username, avatar, roomCode, clientProtocolVersion);
		bool wasCallbackExecuted = false;

		constructAndProcessCTSPacketFromJson(roomJoinRequestPacket.getJson(), [&](ClientToServerPacket& p) {
			wasCallbackExecuted = true;
			auto& returnedPacket = dynamic_cast<RoomJoinRequest_CTSPacket&>(p);
			REQUIRE(returnedPacket.getOwnUsername() == username);
			REQUIRE(returnedPacket.getRoomCode() == roomCode);
			REQUIRE(returnedPacket.getAvatar() == avatar);
			REQUIRE(returnedPacket.getClientProtocolVersion() == clientProtocolVersion);
		});

		REQUIRE(wasCallbackExecuted);
	}
}