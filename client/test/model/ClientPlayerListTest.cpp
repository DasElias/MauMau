#include <model/ClientPlayerList.h>
#include <catch2/catch.hpp>

using namespace card;

TEST_CASE("ClientPlayerList", "[ClientPlayerList]") {
	std::shared_ptr<ParticipantOnClient> localParticipant = std::make_shared<ParticipantOnClient>("A", 1, false);
	std::vector<std::shared_ptr<ParticipantOnClient>> allParticipants = {{
		std::make_shared<ParticipantOnClient>("B", 2, false), localParticipant, std::make_shared<ParticipantOnClient>("C", 3, false)
	}};
	ClientPlayerList playerList(allParticipants, localParticipant);

	// set player on turn
	playerList.setPlayerOnTurn(playerList.lookupPlayer("C"));

	SECTION("removeOpponentLocal") {
		playerList.removeOpponentLocal(playerList.lookupOpponent("B"));
		REQUIRE(playerList.getAmountOfOpponents() == 1);
		REQUIRE(playerList.getAmountOfPlayers() == 2);
	}
	SECTION("remove local participant at removeOpponentLocal") {
		REQUIRE_THROWS(playerList.removeOpponentLocal(playerList.getLocalPlayer()));
	}
	SECTION("lookupPlayer") {
		SECTION("lookupPlayer with local player") {
			auto& p = playerList.lookupPlayer("A");
			REQUIRE(p.getWrappedParticipiant() == localParticipant);
		}
		SECTION("lookupPlayer with opponent") {
			auto& p = playerList.lookupPlayer("B");
			REQUIRE(p.getWrappedParticipiant() == allParticipants[0]);
		}
		SECTION("lookupPlayer with other player") {
			REQUIRE_THROWS(playerList.lookupPlayer("other"));
		}
	}
	SECTION("lookupOpponent") {
		SECTION("lookupOpponent with local player") {
			REQUIRE_THROWS(playerList.lookupOpponent("A"));
		}
		SECTION("lookupOpponent with opponent") {
			auto& p = playerList.lookupOpponent("B");
			REQUIRE(p.getWrappedParticipiant() == allParticipants[0]);
		}
		SECTION("lookupOpponent with other player") {
			REQUIRE_THROWS(playerList.lookupOpponent("other"));
		}
	}
	SECTION("isOpponent") {
		REQUIRE(playerList.isOpponent("B"));	// opponent
		REQUIRE_FALSE(playerList.isOpponent("A"));	// localPlayer
		REQUIRE_FALSE(playerList.isOpponent("random string"));	// random string
	}
	SECTION("isPlayer") {
		REQUIRE(playerList.isPlayer("B"));	// opponent
		REQUIRE(playerList.isPlayer("A"));	// localPlayer
		REQUIRE_FALSE(playerList.isPlayer("random string"));	// random string
	}
	SECTION("isLocalPlayer") {
		REQUIRE(playerList.isLocalPlayer(playerList.getLocalPlayer()));
	}
	SECTION("isPlayerOnTurn") {
		REQUIRE(playerList.isPlayerOnTurn(playerList.lookupPlayer("C")));
		REQUIRE_FALSE(playerList.isPlayerOnTurn(playerList.lookupPlayer("B")));
	}
	SECTION("isLocalPlayerOnTurn") {
		REQUIRE_FALSE(playerList.isLocalPlayerOnTurn());
		playerList.setPlayerOnTurn(playerList.getLocalPlayer());
		REQUIRE(playerList.isLocalPlayerOnTurn());
	}
	SECTION("getLocalPlayer") {
		REQUIRE(playerList.getLocalPlayer() == playerList.lookupPlayer("A"));
		REQUIRE(const_cast<const ClientPlayerList&>(playerList).getLocalPlayer() == playerList.lookupPlayer("A"));
	}
	SECTION("getPlayerOnTurn") {
		REQUIRE(playerList.getPlayerOnTurn() == playerList.lookupPlayer("C"));
	}
	SECTION("getAmountOfPlayers") {
		REQUIRE(playerList.getAmountOfPlayers() == 3);
	}
	SECTION("getAmountOfOpponents") {
		REQUIRE(playerList.getAmountOfOpponents() == 2);
	}
	
}