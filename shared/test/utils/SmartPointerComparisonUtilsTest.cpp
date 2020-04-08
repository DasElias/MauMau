#include <catch2/catch.hpp>
#include <utils/SharedPointerComparisonUtils.h>

using namespace card;

TEST_CASE("SharedPointerComparisonUtils", "[SharedPointerComparisonUtils]") {
	std::shared_ptr<int> ptr = std::make_shared<int>(2);

	SECTION("equals") {
		const int& ref = *ptr;
		bool eq1 = ref == ptr;
		bool eq2 = ptr == ref;
		REQUIRE(eq1);
		REQUIRE(eq1);
	}
	SECTION("not equals") {
		int otherVal = 2;
		bool eq1 = otherVal == ptr;
		bool eq2 = ptr == otherVal;
		REQUIRE_FALSE(eq1);
		REQUIRE_FALSE(eq2);
	}
}