#include <catch/catch.hpp>
#include <model/CardCollection.h>
using namespace card;

TEST_CASE("Constructors of CardCollection", "[CardCollection]") {
	SECTION("Default constructor") {
		CardCollection c;

		REQUIRE(c.isEmpty());
	}
	SECTION("initialized with Cards") {
		std::vector<Card> defaultCards = {
			Card::CLUB_ACE, Card::DIAMOND_THREE, Card::CLUB_FIVE
		};
		CardCollection c = {defaultCards};

		REQUIRE(! c.isEmpty());
		REQUIRE(c.getSize() == defaultCards.size());
		for(int i = 0; i < c.getSize(); i++) {
			REQUIRE(c.get(i) == defaultCards[i]);
		}
	}
}