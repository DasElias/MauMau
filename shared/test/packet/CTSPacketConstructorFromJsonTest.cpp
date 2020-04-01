#include <catch/catch.hpp>
#include <packet/CTSPacketConstructorFromJson.h>
#include <packet/cts/RoomJoinRequest_CTSPacket.h>
#include <optional>
#include <boost/optional.hpp>

using namespace card;

struct FunctorObject {
	std::reference_wrapper<boost::optional<ClientToServerPacket&>> packet;
	FunctorObject(boost::optional<ClientToServerPacket&>& packet) :
		packet(packet) {

	}
	void operator()(ClientToServerPacket& p) {
		packet.get() = p;
	}
	bool hasValue() const {
		return packet.get().has_value();
	}
	ClientToServerPacket& getPacket() {
		return packet.get().get();
	}
};

TEST_CASE("CTSPacketConstructorFromJson", "[CTSPacketConstructorFromJson]") {
	boost::optional<ClientToServerPacket&> packet;
	FunctorObject callback(packet);
	SECTION("call with invalid json #1") {
		std::string json = "";
		constructAndProcessCTSPacketFromJson(json, callback);
		REQUIRE_FALSE(callback.hasValue());
	}
	SECTION("call with invalid json #2") {
		std::string json = "{pType: \"HALLO\"}";
		constructAndProcessCTSPacketFromJson(json, callback);
		REQUIRE_FALSE(callback.hasValue());
	}
	SECTION("call with invalid json #2") {
		std::string json = "{pType: \"401\"}";
		constructAndProcessCTSPacketFromJson(json, callback);
		REQUIRE_FALSE(callback.hasValue());
	}
	SECTION("call with correct json") {
		std::string const username = "Ha";
		RoomCode const roomCode = 212;
		Avatar const avatar = 1;
		RoomJoinRequest_CTSPacket packet(username, avatar, roomCode);
		constructAndProcessCTSPacketFromJson(packet.getJson(), callback);

		REQUIRE(callback.hasValue());
		auto returnedPacket = dynamic_cast<RoomJoinRequest_CTSPacket&>(callback.getPacket());
		REQUIRE(returnedPacket.getOwnUsername() == username);
		REQUIRE(returnedPacket.getRoomCode() == roomCode);
		REQUIRE(returnedPacket.getAvatar() == avatar);
	}
}