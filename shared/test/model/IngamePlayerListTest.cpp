#include <catch/catch.hpp>
#include <model/IngamePlayerList.h>

using namespace card;

struct TestPlayer {
	TestPlayer() {
		static int valCounter = 0;
		val = valCounter++;
	}

	bool operator==(const TestPlayer& other) const {
		return val == other.val;
	}

	int val;
};

TEST_CASE("IngamePlayerList", "[IngamePlayerList]") {
	std::vector<TestPlayer> players(10);
	IngamePlayerList<TestPlayer> ingamePlayerList(players, players[0]);

	SECTION("getPlayerOnTurn") {
		REQUIRE(players[0] == ingamePlayerList.getPlayerOnTurn());
	}
	SECTION("getNextPlayerOnTurn") {
		REQUIRE(players[1] == ingamePlayerList.getNextPlayerOnTurn(Direction::CW));
		REQUIRE(players[9] == ingamePlayerList.getNextPlayerOnTurn(Direction::CCW));

		REQUIRE(players[6] == ingamePlayerList.getNextPlayerOnTurn(Direction::CW, players[5]));
		REQUIRE(players[4] == ingamePlayerList.getNextPlayerOnTurn(Direction::CCW, players[5]));
	}
	SECTION("isPlayerOnTurn") {
		REQUIRE(ingamePlayerList.isPlayerOnTurn(players[0]));
		REQUIRE_FALSE(ingamePlayerList.isPlayerOnTurn(players[1]));
	}
	SECTION("set player on turn") {
		ingamePlayerList.setPlayerOnTurn(players[1]);
		REQUIRE(ingamePlayerList.isPlayerOnTurn(players[1]));
	}
	SECTION("get amount of players") {
		REQUIRE(ingamePlayerList.getAmountOfPlayers() == 10);
	}
	SECTION("appendPlayer") {
		ingamePlayerList.appendPlayer({});
		REQUIRE(ingamePlayerList.getAmountOfPlayers() == 11);
	}
	SECTION("removePlayer") {
		ingamePlayerList.removePlayer(players[1]);
		REQUIRE(ingamePlayerList.getAmountOfPlayers() == 9);
	}
	SECTION("removePlayer with player on turn") {
		REQUIRE_THROWS(ingamePlayerList.removePlayer(players[0]));
	}
	SECTION("get all players") {
		REQUIRE(ingamePlayerList.getAllPlayers() == players);
	}
}