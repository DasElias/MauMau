#include <catch/catch.hpp>
#include <utils/InsertionSort.h>

using namespace card;

TEST_CASE("InsertionSort", "[InsertionSort]") {
	SECTION("insertionSortAscending") {
		std::vector<int> vec;
		insertionSortAscending(vec, 5);
		insertionSortAscending(vec, 1);
		insertionSortAscending(vec, 2);
		insertionSortAscending(vec, 5);
		insertionSortAscending(vec, 10);
		insertionSortAscending(vec, 7);

		int previousValue = 0;
		for(auto& value : vec) {
			REQUIRE(value >= previousValue);
			previousValue = value;
		}

	}
}