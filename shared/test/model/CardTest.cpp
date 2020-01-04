#include <catch/catch.hpp>
#include <model/Card.h>

using namespace card;

TEST_CASE("convert card correctly to index, number and value and vice-verca", "[Card]") {
	SECTION("NULLCARD") {
		auto c = Card::NULLCARD;
		REQUIRE(c.getCardIndex() == CardIndex::NULLINDEX);
		REQUIRE(c.getValue() == CardValue::NULLVALUE);
		REQUIRE(c.getCardNumber() == 0);
	}
	SECTION("HEART_DEUCE") {
		auto c = Card::HEART_DEUCE;
		REQUIRE(c.getCardIndex() == CardIndex::HEART);
		REQUIRE(c.getValue() == CardValue::DEUCE);
	}
	SECTION("HEART_ACE") {
		auto c = Card::HEART_ACE;
		REQUIRE(c.getCardIndex() == CardIndex::HEART);
		REQUIRE(c.getValue() == CardValue::ACE);
	}
	SECTION("SPADE_DEUCE") {
		auto c = Card::SPADE_DEUCE;
		REQUIRE(c.getCardIndex() == CardIndex::SPADE);
		REQUIRE(c.getValue() == CardValue::DEUCE);
	}
	SECTION("SPADE_ACE") {
		auto c = Card::SPADE_ACE;
		REQUIRE(c.getCardIndex() == CardIndex::SPADE);
		REQUIRE(c.getValue() == CardValue::ACE);
	}
	SECTION("DIAMOND_DEUCE") {
		auto c = Card::DIAMOND_DEUCE;
		REQUIRE(c.getCardIndex() == CardIndex::DIAMOND);
		REQUIRE(c.getValue() == CardValue::DEUCE);
	}
	SECTION("DIAMOND_ACE") {
		auto c = Card::DIAMOND_ACE;
		REQUIRE(c.getCardIndex() == CardIndex::DIAMOND);
		REQUIRE(c.getValue() == CardValue::ACE);
	}
	SECTION("CLUB_DEUCE") {
		auto c = Card::CLUB_DEUCE;
		REQUIRE(c.getCardIndex() == CardIndex::CLUB);
		REQUIRE(c.getValue() == CardValue::DEUCE);
	}
	SECTION("CLUB_ACE") {
		auto c = Card::CLUB_ACE;
		REQUIRE(c.getCardIndex() == CardIndex::CLUB);
		REQUIRE(c.getValue() == CardValue::ACE);
	}
}
TEST_CASE("comparison operators", "[Card]") {
	SECTION("comparison operators") {
		REQUIRE(Card::CLUB_DEUCE == Card::CLUB_DEUCE);
		REQUIRE(Card::CLUB_DEUCE < Card::CLUB_ACE);
		REQUIRE(Card::CLUB_ACE > Card::CLUB_DEUCE);
		REQUIRE(Card::CLUB_DEUCE <= Card::CLUB_DEUCE);
		REQUIRE(Card::CLUB_DEUCE >= Card::CLUB_DEUCE);
	}
}