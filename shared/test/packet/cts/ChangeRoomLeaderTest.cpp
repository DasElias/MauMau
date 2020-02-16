#include <catch/catch.hpp>
#include <packet/cts/ChangeRoomLeaderRequest_CTSPacket.h>
using namespace card;

TEST_CASE("ChangeRoomLeaderRequest_CTSPacket", "[ChangeRoomLeaderRequest_CTSPacket]") {
	std::string const newLeader = "Hello world";

	ChangeRoomLeaderRequest_CTSPacket packet(newLeader);
	nlohmann::json json = nlohmann::json::parse(packet.getJson());
	ChangeRoomLeaderRequest_CTSPacket jsonPacket(json);

	REQUIRE(newLeader == jsonPacket.getUsernameOfNewLeader());
}