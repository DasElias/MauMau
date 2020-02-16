#include <catch/catch.hpp>
#include <packet/stc/EnteringRoomSuccessReport_STCAnswerPacket.h>
using namespace card;

TEST_CASE("EnteringRoomSuccessReport_STCAnswerPacket", "[EnteringRoomSuccessReport_STCAnswerPacket]") {
	int const statusCode = 5;
	RoomCode const roomCode = 10;
	std::vector<std::string> const usernamesOfOtherParticipants = {{"Hey", "Test", "Hello World"}};
	std::vector<Avatar> const avatarsOfOtherParticipants = {{4, 6, 8}};
	std::string const roomLeader = "roomLeader"; 
	std::map<std::string, int> options;
	options["Test"] = 1;

	EnteringRoomSuccessReport_STCAnswerPacket packet(statusCode, usernamesOfOtherParticipants, avatarsOfOtherParticipants, roomLeader, roomCode, options);
	nlohmann::json json = nlohmann::json::parse(packet.getJson());
	EnteringRoomSuccessReport_STCAnswerPacket jsonPacket(json);

	REQUIRE(statusCode == jsonPacket.getStatusCode());
	REQUIRE(roomCode == jsonPacket.getRoomCode());
	REQUIRE(usernamesOfOtherParticipants == jsonPacket.getUsernamesOfOtherParticipants());
	REQUIRE(avatarsOfOtherParticipants == jsonPacket.getAvatarsOfOtherParticipants());
	REQUIRE(roomLeader == jsonPacket.getRoomLeader());
	REQUIRE(options == jsonPacket.getOptions());
} 