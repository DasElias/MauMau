#include <catch2/catch.hpp>
#include <model/ParticipantOnClient.h>

using namespace card;

TEST_CASE("ParticipantOnClient", "[ParticipantOnClient]") {
	const std::string username = "Testnutzer";
	ParticipantOnClient p(username, 0, false);

	SECTION("getUsername") {
		REQUIRE(p.getUsername() == username);
	}
}