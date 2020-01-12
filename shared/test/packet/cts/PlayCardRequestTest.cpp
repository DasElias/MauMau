#include <catch/catch.hpp>
#include <packet/cts/PlayCardRequest_CTSPacket.h>
using namespace card;

TEST_CASE("PlayCardRequest_CTSPacket", "[PlayCardRequest_CTSPacket]") {
	int const card = 0;
	int const newCardIndex = 1;

	PlayCardRequest_CTSPacket packet(card, newCardIndex);
	PlayCardRequest_CTSPacket jsonPacket(packet.getJson());

	REQUIRE(card == jsonPacket.getCardNumber());
	REQUIRE(newCardIndex == jsonPacket.getCardNumber());
}