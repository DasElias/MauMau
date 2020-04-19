#include <catch2/catch.hpp>
#include <model/ShouldDisplayTutorial.h>
#include "../system/DeleteRegistryKey.h"

using namespace card;

static std::wstring const PARENT = L"Software\\MauMau 3D";
static std::wstring const SUBKEY = L"shoulddisplaytutorialtest";
static std::wstring const SUBKEY_COMBINED = PARENT + L"\\" + SUBKEY;

TEST_CASE("ShouldDisplayTutorial", "[ShouldDisplayTutorial]") {
	deleteRegistryKey(PARENT, SUBKEY);
	ShouldDisplayTutorial shouldDisplayTutorial(SUBKEY_COMBINED);

	SECTION("shouldDisplayTutorial") {
		REQUIRE(shouldDisplayTutorial.shouldDisplay(TutorialMessage::CHOOSE_COLOR_EXPLANATION));
	}
	SECTION("shouldDisplayAndClear") {
		REQUIRE(shouldDisplayTutorial.shouldDisplayAndClear(TutorialMessage::CHOOSE_COLOR_EXPLANATION));
		REQUIRE_FALSE(shouldDisplayTutorial.shouldDisplayAndClear(TutorialMessage::CHOOSE_COLOR_EXPLANATION));
		
		// query two more times to check if the value cache does work as intended
		REQUIRE_FALSE(shouldDisplayTutorial.shouldDisplayAndClear(TutorialMessage::CHOOSE_COLOR_EXPLANATION));
		REQUIRE_FALSE(shouldDisplayTutorial.shouldDisplayAndClear(TutorialMessage::CHOOSE_COLOR_EXPLANATION));
	}
	SECTION("clear") {
		shouldDisplayTutorial.clear(TutorialMessage::CHOOSE_COLOR_EXPLANATION);
		REQUIRE_FALSE(shouldDisplayTutorial.shouldDisplay(TutorialMessage::CHOOSE_COLOR_EXPLANATION));
	
		// query two more times to check if the value cache does work as intended
		REQUIRE_FALSE(shouldDisplayTutorial.shouldDisplay(TutorialMessage::CHOOSE_COLOR_EXPLANATION));
		REQUIRE_FALSE(shouldDisplayTutorial.shouldDisplay(TutorialMessage::CHOOSE_COLOR_EXPLANATION));
	}
}