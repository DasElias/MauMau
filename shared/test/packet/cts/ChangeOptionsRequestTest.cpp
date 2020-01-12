#include <catch/catch.hpp>
#include <packet/cts/ChangeOptionsRequest_CTSPacket.h>
using namespace card;

TEST_CASE("ChangeOptionsRequest_CTSPacket", "[ChangeOptionsRequest_CTSPacket]") {
	std::map<std::string, int> options;
	options["Test"] = 1;

	ChangeOptionsRequest_CTSPacket packet(options);
	ChangeOptionsRequest_CTSPacket jsonPacket(packet.getJson());

	REQUIRE(options == jsonPacket.getOptions());
}