#include <catch2/catch.hpp>
#include <packet/cts/PlayCardRequest_CTSPacket.h>
using namespace card;

TEST_CASE("PlayCardRequest_CTSPacket", "[PlayCardRequest_CTSPacket]") {
	int const card = 0;
	bool const wasJustDrawn = false;
	int const newCardIndex = 1;

	PlayCardRequest_CTSPacket packet(card, wasJustDrawn, newCardIndex);
	nlohmann::json json = nlohmann::json::parse(packet.getJson());
	PlayCardRequest_CTSPacket jsonPacket(json);

	REQUIRE(card == jsonPacket.getCardNumber());	
	REQUIRE(wasJustDrawn == jsonPacket.wasJustDrawn());
	REQUIRE(newCardIndex == jsonPacket.getNewCardIndex());
}