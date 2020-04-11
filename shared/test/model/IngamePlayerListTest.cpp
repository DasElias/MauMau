#include <catch2/catch.hpp>
#include <model/IngamePlayerList.h>
#include <utils/SharedPointerComparisonUtils.h>

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
	std::vector<std::shared_ptr<TestPlayer>> players;
	for(int i = 0; i < 10; i++) {
		players.push_back(std::make_shared<TestPlayer>());
	}
	IngamePlayerList<TestPlayer> ingamePlayerList(players, players[0]);

	SECTION("getPlayerOnTurn") {
		bool cond = *players[0] == ingamePlayerList.getPlayerOnTurn();
		REQUIRE(cond);
	}
	SECTION("getNextPlayerOnTurn") {
		bool c1 = players[1] == ingamePlayerList.getNextPlayerOnTurn(Direction::CW);
		REQUIRE(c1);
		bool c2 = players[9] == ingamePlayerList.getNextPlayerOnTurn(Direction::CCW);
		REQUIRE(c2);

		bool c3 = players[6] == ingamePlayerList.getNextPlayerOnTurn(Direction::CW, *players[5]);
		REQUIRE(c3);
		bool c4 = players[4] == ingamePlayerList.getNextPlayerOnTurn(Direction::CCW, *players[5]);
		REQUIRE(c4);
	}
	SECTION("isPlayerOnTurn") {
		REQUIRE(ingamePlayerList.isPlayerOnTurn(*players[0]));
		REQUIRE_FALSE(ingamePlayerList.isPlayerOnTurn(*players[1]));
	}
	SECTION("set player on turn") {
		ingamePlayerList.setPlayerOnTurn(*players[1]);
		REQUIRE(ingamePlayerList.isPlayerOnTurn(*players[1]));
	}
	SECTION("get amount of players") {
		REQUIRE(ingamePlayerList.getAmountOfPlayers() == 10);
	}
	SECTION("appendPlayer") {
		ingamePlayerList.appendPlayer({});
		REQUIRE(ingamePlayerList.getAmountOfPlayers() == 11);
	}
	SECTION("removePlayer") {
		ingamePlayerList.removePlayer(*players[1]);
		REQUIRE(ingamePlayerList.getAmountOfPlayers() == 9);
	}
	SECTION("removePlayer with player on turn") {
		REQUIRE_THROWS(ingamePlayerList.removePlayer(*players[0]));
	}
	SECTION("get all players") {
		REQUIRE(ingamePlayerList.getAllPlayers() == players);
	}
	SECTION("begin") {
		REQUIRE(**players.begin() == *ingamePlayerList.begin());
		REQUIRE(**players.begin() == *(const_cast<const IngamePlayerList<TestPlayer>&>(ingamePlayerList).begin()));
		REQUIRE(**players.begin() == *(const_cast<const IngamePlayerList<TestPlayer>&>(ingamePlayerList).cbegin()));
	}
	SECTION("end") {
		REQUIRE(**(players.end() - 1) == *(ingamePlayerList.end() - 1));
		REQUIRE(**(players.end() - 1) == *(const_cast<const IngamePlayerList<TestPlayer>&>(ingamePlayerList).end() - 1));
		REQUIRE(**(players.end() - 1) == *(const_cast<const IngamePlayerList<TestPlayer>&>(ingamePlayerList).cend() - 1));
	}
}