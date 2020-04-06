#include <catch2/catch.hpp>
#include <model/CardAnimation.h>
#include <model/CardAnimator.h>
#include <shared/model/HandCardStack.h>

using namespace card;

TEST_CASE("CardAnimation", "[CardAnimation]") {
	SECTION("comparison") {
		CardAnimator cardStack(std::make_unique<HandCardStack>());
		CardAnimation a(0, 1000, cardStack, Card::NULLCARD, 0);
		CardAnimation b(1000, 1000, cardStack, Card::NULLCARD, 0);

		REQUIRE(a < b);
	}
}