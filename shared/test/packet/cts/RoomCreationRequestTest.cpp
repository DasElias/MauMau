#include <catch/catch.hpp>
#include <packet/cts/RoomCreationRequest_CTSPacket.h>
using namespace card;

TEST_CASE("RoomCreationRequest_CTSPacket", "[RoomCreationRequest_CTSPacket]") {
	std::string const username = "Test";
	Avatar const avatar = 0;

	RoomCreationRequest_CTSPacket packet(username, avatar);
	RoomCreationRequest_CTSPacket jsonPacket(packet.getJson());

	REQUIRE(username == jsonPacket.getOwnUsername());
	REQUIRE(avatar == jsonPacket.getAvatar());
}