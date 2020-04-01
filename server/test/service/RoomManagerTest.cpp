#include <catch/catch.hpp>
#include <service/RoomManager.h>
#include "AbstractConnectionToClientDummy.h"
#include "AbstractServerPacketTransmitterDummy.h"

using namespace card;

TEST_CASE("RoomManager", "RoomManager") {
	std::shared_ptr<AbstractServerPacketTransmitterDummy> packetTransmitter = std::make_shared<AbstractServerPacketTransmitterDummy>();
	RoomManager roomManager(packetTransmitter);

	SECTION("create room") {
		std::string const leaderName = "Hallo Welt";
		Avatar const leaderAvatar = 1;
		auto const leaderConn = std::make_shared<AbstractConnectionToClientDummy>();
		RoomOptions const options;

		auto createReturnValue = roomManager.createAndJoin(leaderName, leaderAvatar, options.getAllOptions(), leaderConn);

		REQUIRE(createReturnValue.constructedParticipant->getUsername() == leaderName);
		REQUIRE(createReturnValue.constructedParticipant->getAvatar() == leaderAvatar);
		REQUIRE(createReturnValue.room.getRoomOptions().getAllOptions() == options.getAllOptions());

		REQUIRE(packetTransmitter->wasParticipantRegistered(leaderConn));
		REQUIRE(roomManager.isConnectionInRoom(leaderConn));
		REQUIRE(roomManager.getRoomToConnection(leaderConn) == createReturnValue.room);
		REQUIRE(roomManager.getParticipantToConnection(leaderConn) == createReturnValue.constructedParticipant);
		REQUIRE(roomManager.getConnectionToParticipant(createReturnValue.constructedParticipant) == leaderConn);
		
		RoomCode roomCode = createReturnValue.room.getRoomCode();

		SECTION("join") {
			std::string const userName = "Test";
			Avatar const userAvatar = 2;
			auto const userConn = std::make_shared<AbstractConnectionToClientDummy>();

			auto joinReturnValue = roomManager.join(roomCode, userName, userAvatar, userConn);

			REQUIRE(packetTransmitter->wasParticipantRegistered(userConn));
			REQUIRE(roomManager.isConnectionInRoom(userConn));
			REQUIRE(roomManager.getRoomToConnection(userConn) == joinReturnValue.room);
			REQUIRE(roomManager.getParticipantToConnection(userConn) == joinReturnValue.constructedParticipant);
			REQUIRE(roomManager.getConnectionToParticipant(joinReturnValue.constructedParticipant) == userConn);
			REQUIRE(joinReturnValue.room.getRoomCode() == roomCode);

			SECTION("kick") {
				SECTION("user tries to kick leader") {
					REQUIRE_FALSE(roomManager.kick(leaderName, userConn));
				}
				SECTION("leader tries to kick non-existing user") {
					REQUIRE_FALSE(roomManager.kick("AAAAA", leaderConn));
				}
				SECTION("non-existing conn tries to kick an user") {
					auto const nonExistingConn = std::make_shared<AbstractConnectionToClientDummy>();
					REQUIRE_FALSE(roomManager.kick(userName, nonExistingConn));
				}
				SECTION("successful kick") {
					REQUIRE(roomManager.kick(userName, leaderConn));
					REQUIRE(createReturnValue.room.getUsernamesOfParticipants().size() == 1);
					REQUIRE_FALSE(packetTransmitter->wasParticipantRegistered(userConn));
					REQUIRE_FALSE(roomManager.isConnectionInRoom(userConn));
					REQUIRE_THROWS(roomManager.getRoomToConnection(userConn));
					REQUIRE_THROWS(roomManager.getParticipantToConnection(userConn));
					REQUIRE_THROWS(roomManager.getConnectionToParticipant(joinReturnValue.constructedParticipant));
				}
			}
			SECTION("leave") {
				SECTION("try to leave with non-existing connection") {
					auto const nonExistingConn = std::make_shared<AbstractConnectionToClientDummy>();
					roomManager.leave(nonExistingConn);
				}
				SECTION("successful leave") {
					roomManager.leave(userConn);
					REQUIRE(createReturnValue.room.getUsernamesOfParticipants().size() == 1);
					REQUIRE_FALSE(packetTransmitter->wasParticipantRegistered(userConn));
					REQUIRE_FALSE(roomManager.isConnectionInRoom(userConn));
					REQUIRE_THROWS(roomManager.getRoomToConnection(userConn));
					REQUIRE_THROWS(roomManager.getParticipantToConnection(userConn));
					REQUIRE_THROWS(roomManager.getConnectionToParticipant(joinReturnValue.constructedParticipant));
				}
			}
		}
		SECTION("join with too much users") {
			roomManager.join(roomCode, "1", 3, std::make_shared<AbstractConnectionToClientDummy>());
			roomManager.join(roomCode, "2", 3, std::make_shared<AbstractConnectionToClientDummy>());
			roomManager.join(roomCode, "3", 3, std::make_shared<AbstractConnectionToClientDummy>());
			REQUIRE_THROWS(roomManager.join(roomCode, "4", 3, std::make_shared<AbstractConnectionToClientDummy>()));

		}
		SECTION("join with duplicate username") {
			REQUIRE_THROWS(roomManager.join(roomCode, leaderName, leaderAvatar, std::make_shared<AbstractConnectionToClientDummy>()));
		}
		SECTION("join with duplicate connection") {
			REQUIRE_THROWS(roomManager.join(roomCode, "A", 1, leaderConn));
		}
		SECTION("create with duplicate connection") {
			REQUIRE_THROWS(roomManager.createAndJoin(leaderName, leaderAvatar, options.getAllOptions(), leaderConn));
		}
	}
}