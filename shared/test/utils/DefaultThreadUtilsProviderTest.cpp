#include <catch/catch.hpp>
#include "../shared/utils/DefaultThreadUtilsProvider.h"

using namespace card;

TEST_CASE("DefaultThreadUtilsProvider", "[DefaultThreadUtilsProvider]") {
	DefaultThreadUtilsProvider provider;

	SECTION("callback is executed") {
		bool wasExecuted = false;
		provider.invokeIn(0, nullptr, [&wasExecuted]() {
			wasExecuted = true;
		});
		provider.update();
		REQUIRE(wasExecuted);
	}
	SECTION("callback isn't executed twice") {
		bool wasExecuted = false;
		provider.invokeIn(0, nullptr, [&wasExecuted]() {
			wasExecuted = true;
		});
		provider.update();
		REQUIRE(wasExecuted);
		wasExecuted = false;
		provider.update();
		REQUIRE(! wasExecuted);
	}
	SECTION("callback in a few seconds is not executed") {
		bool wasExecuted = false;
		provider.invokeIn(10000, nullptr, [&wasExecuted]() {
			wasExecuted = true;
		});
		provider.update();
		REQUIRE(! wasExecuted);
	}
	SECTION("insert callback in callback") {
		provider.invokeIn(0, nullptr, [&provider]() {
			provider.invokeIn(0, nullptr, []() {
				
			});
		});
		provider.update();
	}
	SECTION("insert callback with key") {
		int keyValue;
		bool wasExecuted = false;
		provider.invokeIn(0, &keyValue, [&wasExecuted]() {
			wasExecuted = true;
		});
		provider.removeCallbacks(&keyValue);
		provider.update();
		REQUIRE(! wasExecuted);
	}
	SECTION("callback with another key isn't deleted") {
		int keyValue1;
		int keyValue2;
		bool wasExecuted = false;
		provider.invokeIn(0, &keyValue1, [&wasExecuted]() {
			wasExecuted = true;
		});
		provider.invokeIn(0, &keyValue2, []() {});
		provider.removeCallbacks(&keyValue2);
		provider.update();
		REQUIRE(wasExecuted);
	}

}