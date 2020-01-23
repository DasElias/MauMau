#include <catch/catch.hpp>
#include <model/IntegerOptions.h>

using namespace card;

TEST_CASE("IntegerOptionsTest getters/setters", "[IntegerOptionsTest]") {
	IntegerOptions options;

	SECTION("set/query option") {
		std::string const key = "testkey";
		REQUIRE_THROWS(options.queryOption(key));
		options.setOption(key, 1);
		REQUIRE(1 == options.queryOption(key));
	}
	SECTION("query option with default value") {
		std::string const key = "testkey";
		REQUIRE(0 == options.queryOption(key, 0));
		options.setOption(key, 1);
		REQUIRE(1 == options.queryOption(key));
	}
	SECTION("setOptionIfNotExist") {
		std::string const key = "testkey";
		options.setOptionIfNotExist(key, 1);
		REQUIRE(1 == options.queryOption(key));
		options.setOptionIfNotExist(key, 2);
		REQUIRE(1 == options.queryOption(key));
	}
	SECTION("doesOptionExist") {
		std::string const key = "testkey";
		REQUIRE(! options.doesOptionExist(key));
		options.setOption(key, 1);
		REQUIRE(options.doesOptionExist(key));
	}
	SECTION("get/set data") {
		std::string const key = "testkey";
		std::map<std::string, int> map;
		map[key] = 1;
		options.setAllOptions(map);

		REQUIRE(options.doesOptionExist(key));
		REQUIRE(map == options.getData());
	}
}	