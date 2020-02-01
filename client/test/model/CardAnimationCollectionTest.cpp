#include <catch/catch.hpp>
#include <model/CardAnimationCollection.h>
#include <shared/model/CardStack.h>
#include <model/CardAnimator.h>

using namespace card;

TEST_CASE("CardAnimationCollection", "[CardAnimationCollection]") {
	CardAnimationCollection set;
	CardAnimator source(std::make_unique<CardStack>());

	SECTION("exeption when trying to get card from empty collection") {
		REQUIRE_THROWS(set.getFirst());
		REQUIRE_THROWS(set.getLast());
	}
	SECTION("insertAnimation should add elements") {
		CardAnimation anim = {0, 0, source, Card(0)};
		set.insertAnimation(anim);
		set.insertAnimation(anim);
		REQUIRE(set.size() == 2);
	}
	SECTION("removeAnimation() should remove only one single animation") {
		CardAnimation anim = {0, 0, source, Card(0)};
		set.insertAnimation(anim);
		set.insertAnimation(anim);
		set.removeAnimation(anim);
		REQUIRE(set.size() == 1);
	}
	SECTION("get") {
		long long startTime0 = 0;
		long long startTime1 = 1000;
		int duration0 = 2000;
		int duration1 = 100;

		set.insertAnimation({startTime0, duration0, source, Card(0)});
		set.insertAnimation({startTime1, duration1, source, Card(0)});

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