#include <catch/catch.hpp>
#include <model/MauMauPunishmentMessageGenerator.h>

using namespace card;

#define CHECK_MESSAGE(username, output) REQUIRE(output.find(username.c_str()) != std::string::npos)

TEST_CASE("MauMauPunishmentMessageGenerator", "[MauMauPunishmentMessageGenerator]") {
	SECTION("generateMessage") {
		std::string const username = "HALLOWELT";
		CHECK_MESSAGE(username, MauMauPunishmentMessageGenerator::generateMessage(username, MauPunishmentCause::DRAWED_EVEN_THOUGH_MAUED));
		CHECK_MESSAGE(username, MauMauPunishmentMessageGenerator::generateMessage(username, MauPunishmentCause::NO_MAU_RECEIVED));
		CHECK_MESSAGE(username, MauMauPunishmentMessageGenerator::generateMessage(username, MauPunishmentCause::TOO_EARLY));
	}
	SECTION("generateMessages with invalid parameter") {
		std::string output = MauMauPunishmentMessageGenerator::generateMessage("USERNAME", static_cast<MauPunishmentCause>(500));
		REQUIRE(output == "");
	}
	SECTION("appendMessage") {
		MessageQueue mq;
		MauMauPunishmentMessageGenerator::appendMessage(mq, "ABC", MauPunishmentCause::TOO_EARLY);
		REQUIRE(mq.getLastVisibleMessages().size() == 1);
	}
}