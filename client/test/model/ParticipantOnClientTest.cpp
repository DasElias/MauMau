#include <catch/catch.hpp>
#include <model/ParticipantOnClient.h>

using namespace card;

TEST_CASE("ParticipantOnClient", "[ParticipantOnClient]") {
	const std::string username = "Testnutzer";
	ParticipantOnClient p(username);

	SECTION("getUsername") {
		REQUIRE(p.getUsername() == username);
	}
}