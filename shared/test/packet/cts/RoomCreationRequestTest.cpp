#include <catch/catch.hpp>
#include <packet/cts/RoomCreationRequest_CTSPacket.h>
using namespace card;

TEST_CASE("RoomCreationRequest_CTSPacket", "[RoomCreationRequest_CTSPacket]") {
	std::string const username = "Test";
	Avatar const avatar = 0;

	RoomCreationRequest_CTSPacket packet(username, avatar);
	nlohmann::json json = nlohmann::json::parse(packet.getJson());
	RoomCreationRequest_CTSPacket jsonPacket(json);

	REQUIRE(username == jsonPacket.getOwnUsername());
	REQUIRE(avatar == jsonPacket.getAvatar());
}