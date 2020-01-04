#include <catch/catch.hpp>
#include "../shared/utils/ReferenceOrValueWrapper.h"

using namespace card;

class TestStruct {
	public:
		TestStruct(int value) :
			value(value) {

		}
		TestStruct(const TestStruct&) = delete;
		TestStruct& operator=(const TestStruct&) = delete;
		bool operator==(const TestStruct& other) const {
			return this->value == other.value;
		}

		int value;
};

TEST_CASE("Insertion in ReferenceOrValueWrapper", "[ReferenceOrValueWrapper]") {
	SECTION("by reference") {
		TestStruct value(5);
		ReferenceOrValueWrapper<TestStruct> wrapper(value);
		REQUIRE(value == wrapper.get());
	}
	SECTION("by value") {
		ReferenceOrValueWrapper<TestStruct> w(1);
		REQUIRE(w.get().value == 1);
	}

}