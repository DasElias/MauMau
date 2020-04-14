#include <catch2/catch.hpp>
#include <model/ProxyMauMauGame.h>
#include <shared/model/AvatarUtils.h>
#include <shared/utils/ThreadUtils.h>
#include <shared/utils/InstantExecutionThreadUtilsProvider.h>
using namespace card;

TEST_CASE("ProxyMauMauGameData", "[ProxyMauMauGameData]") {
	threadUtils_setProvider(std::make_unique<InstantExecutionThreadUtilsProvider>());
	
	std::shared_ptr<ParticipantOnClient> const localParticipant = std::make_shared<ParticipantOnClient>("Lokaler Spieler", getRandomMaleAvatar(), false);
	std::vector<int> const handCards = {{1, 2, 3, 4, 5, 6}};
	std::shared_ptr<ParticipantOnClient> const opponent1 = std::make_shared<ParticipantOnClient>("Opponent1", getRandomMaleAvatar(), false);
	std::shared_ptr<ParticipantOnClient> const opponent2 = std::make_shared<ParticipantOnClient>("Opponent2", getRandomMaleAvatar(), false);
	std::vector<std::shared_ptr<ParticipantOnClient>> const allParticipants{{localParticipant, opponent1, opponent2}};
	int const lastCardOnPlayStack = 7;
	int const lastCardOnDrawCardStack = 8;
	RoomOptions options;

	bool wasTurnEndCallbackInvoked = false;
	ProxyMauMauGameData gameData(allParticipants, localParticipant, handCards, lastCardOnPlayStack, options);
	gameData.getTurnEndEventManager().addEventHandler([&](ProxyPlayer&) {
		wasTurnEndCallbackInvoked = true;
	});
	gameData.setInitialPlayerOnTurnLocal(gameData.lookupPlayer(localParticipant->getUsername()), Card(7));

	SECTION("getPlayStack") {
		auto& playCardStack = gameData.getPlayStack();
		REQUIRE(playCardStack.getLast().getCardNumber() == lastCardOnPlayStack);
	}
	SECTION("getLocalPlayer") {
		auto& localPlayer = gameData.getLocalPlayer();
		REQUIRE(localPlayer.getWrappedParticipiant() == localParticipant);
	}
	/*SECTION("change player on turn") {
		REQUIRE(gameData.isLocalPlayerOnTurn());

		SECTION("set next player on turn") {
			gameData.setNextPlayerOnTurnLocal();
			REQUIRE(gameData.getPlayerOnTurn()->getWrappedParticipiant() == opponent1);
			REQUIRE(! gameData.isLocalPlayerOnTurn());
		}
		SECTION("set next but one player on turn") {
			gameData.setNextButOnePlayerOnTurnLocal();
			REQUIRE(gameData.getPlayerOnTurn()->getWrappedParticipiant() == opponent2);
		}
		SECTION("set next or next but one player on turn") {
			gameData.setNextOrNextButOneOnTurnLocal(Card::CLUB_EIGHT);
			REQUIRE(gameData.getPlayerOnTurn()->getWrappedParticipiant() == opponent2);
			gameData.setNextOrNextButOneOnTurnLocal(Card::CLUB_SEVEN);
			REQUIRE(gameData.isLocalPlayerOnTurn());
		}
		
		REQUIRE(wasTurnEndCallbackInvoked);

	}*/
	SECTION("player has to draw cards") {
		auto& player = gameData.lookupPlayer(opponent1->getUsername());
		std::size_t handCardSizeBefore = player.getCardStack().getSize();
		gameData.playerHasToDrawCards(player, 5);
		REQUIRE(player.getCardStack().getSize() == handCardSizeBefore + 5);
	}
	SECTION("local player plays card") {
		auto& localPlayer = gameData.getLocalPlayer();

		SECTION("local player tries to play card not in he's hand cards") {
			REQUIRE_THROWS(
				gameData.playCardFromHandCards(localPlayer, Card(7), CardIndex::NULLINDEX)
			);
		}
	}
	SECTION("user management methods") {
		SECTION("getOpponents") {
			auto opponentsVector = gameData.getOpponents();
			REQUIRE(opponentsVector.size() == 2);
		}
		SECTION("checkIfOpponents") {
			REQUIRE(! gameData.checkIfIsOpponent(localParticipant->getUsername()));
			REQUIRE(gameData.checkIfIsOpponent(opponent1->getUsername()));
			REQUIRE(! gameData.checkIfIsOpponent("abcdef"));
		}
		SECTION("lookupPlayer") {
			REQUIRE_THROWS(gameData.lookupPlayer("abcdef"));
			REQUIRE(gameData.lookupPlayer(localParticipant->getUsername()).getWrappedParticipiant() == localParticipant);
			REQUIRE(gameData.lookupPlayer(opponent1->getUsername()).getWrappedParticipiant() == opponent1);
		}
		SECTION("lookupOpponent") {
			REQUIRE_THROWS(gameData.lookupOpponent("abcdef"));
			REQUIRE_THROWS(gameData.lookupOpponent(localParticipant->getUsername()).getWrappedParticipiant());
			REQUIRE(gameData.lookupOpponent(opponent1->getUsername()).getWrappedParticipiant() == opponent1);
		}
		SECTION("remove opponent") {
			REQUIRE_THROWS(gameData.removeOpponentLocal(*localParticipant));
			REQUIRE_THROWS(gameData.removeOpponentLocal(
				*std::make_shared<ParticipantOnClient>("abc", 1, false)
			));
			
			auto& opponent1Player = gameData.lookupOpponent(opponent1->getUsername());
			gameData.setOnTurnLocal(opponent1Player);
			gameData.removeOpponentLocal(*opponent1);

			REQUIRE(gameData.getOpponents().size() == 1);
			REQUIRE(gameData.getPlayerOnTurn() != opponent1Player);
		}
	}

}