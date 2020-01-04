#include <catch/catch.hpp>
#include <utils/MathUtils.h>

using namespace card;

TEST_CASE("randomInRange") {
	for(int i = 0; i < 20; i++) {
		int rand = randomInRange(1, 2);
		REQUIRE(rand >= 1);
		REQUIRE(rand <= 2);
	}
}
TEST_CASE("avg") {
	REQUIRE(avg(0, 10) == 5);
}