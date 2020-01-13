#include <catch/catch.hpp>
#include <packet/cts/RoomJoinRequest_CTSPacket.h>
using namespace card;

TEST_CASE("RoomJoinRequest_CTSPacket", "[RoomJoinRequest_CTSPacket]") {
	std::string const username = "Test";
	Avatar const avatar = 0;
	RoomCode const roomCode = 0;

	RoomJoinRequest_CTSPacket packet(username, avatar, roomCode);
	nlohmann::json json = nlohmann::json::parse(packet.getJson());
	RoomJoinRequest_CTSPacket jsonPacket(json);

	REQUIRE(username == jsonPacket.getOwnUsername());
	REQUIRE(avatar == jsonPacket.getAvatar());
	REQUIRE(roomCode == jsonPacket.getRoomCode());
}