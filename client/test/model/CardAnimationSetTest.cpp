#include <catch/catch.hpp>
#include <model/CardAnimationSet.h>
#include <shared/model/CardStack.h>
#include <model/CardAnimator.h>

using namespace card;

TEST_CASE("CardAnimationSet", "[CardAnimationSet]") {
	CardAnimationSet set;
	CardAnimator source(std::make_unique<CardStack>());

	SECTION("exeption when trying to get card from empty set") {
		REQUIRE_THROWS(set.getFirst());
		REQUIRE_THROWS(set.getLast());
	}
	SECTION("get") {
		long long startTime0 = 0;
		long long startTime1 = 1000;
		int duration0 = 2000;
		int duration1 = 100;

		set.insert({startTime0, duration0, source, Card(0)});
		set.insert({startTime1, duration1, source, Card(0)});

		SECTION("getFirst") {
			// animation 0 would be added first to playCardStack
			REQUIRE(set.getFirst().animationStartTime == startTime0);
		}
		SECTION("getLast") {
			// animation 0 would be added first to playCardStack
			REQUIRE(set.getLast().animationStartTime == startTime1);
		}
	
	}
}