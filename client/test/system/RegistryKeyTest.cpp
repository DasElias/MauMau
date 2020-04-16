#include <system/RegistryKey.h>
#include <catch2/catch.hpp>
#include <Windows.h>
#include <iostream>

using namespace card;

static std::wstring PARENT = L"Software\\MauMau 3D";
static std::wstring SUBKEY = L"Software\\MauMau 3D\\unittest";

void deleteRegistryKey() {
	HKEY key;
	if(RegCreateKeyExW(HKEY_CURRENT_USER, PARENT.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, NULL) != ERROR_SUCCESS) {
		FAIL();
	}
	LSTATUS ec = RegDeleteKeyW(key, L"unittest");
	if(ec != ERROR_SUCCESS && ec != ERROR_FILE_NOT_FOUND) {
		FAIL();
	}
	RegCloseKey(key);
}

TEST_CASE("RegistryKey", "[RegistryKey]") {
	SECTION("non-existing RegistryKey") {
		deleteRegistryKey();
		RegistryKey registryKey(SUBKEY);

		SECTION("wasCreatedNewKey") {
			REQUIRE(registryKey.wasCreatedNewKey());
		}
		SECTION("DWORD") {
			std::wstring const VALUE_KEY = L"dwordkey";
			std::uint32_t const DEFAULT_VALUE = 20;
			std::uint32_t const VALUE = 10;

			// getDword 
			REQUIRE_THROWS(registryKey.getDword(VALUE_KEY));

			// getDwordOrDefault
			REQUIRE(registryKey.getDwordOrDefault(VALUE_KEY, DEFAULT_VALUE) == DEFAULT_VALUE);

			// setDword
			registryKey.setDword(VALUE_KEY, VALUE);

			// getDword
			REQUIRE(registryKey.getDword(VALUE_KEY) == VALUE);
			
			// getDwordOrDefault
			REQUIRE(registryKey.getDwordOrDefault(VALUE_KEY, DEFAULT_VALUE) == VALUE);
		}
		SECTION("String") {
			std::wstring const VALUE_KEY = L"stringKey";
			std::string const DEFAULT_VALUE = "defaultkey";
			std::string const VALUE = "dfdf";

			// getString
			REQUIRE_THROWS(registryKey.getString(VALUE_KEY));

			// getStringOrDefault
			REQUIRE(registryKey.getStringOrDefault(VALUE_KEY, DEFAULT_VALUE) == DEFAULT_VALUE);

			// setString
			registryKey.setString(VALUE_KEY, VALUE);

			// getString
			REQUIRE(registryKey.getString(VALUE_KEY) == VALUE);

			// getStringOrDefault
			REQUIRE(registryKey.getStringOrDefault(VALUE_KEY, DEFAULT_VALUE) == VALUE);

		}
		SECTION("edge case for string") {
			std::wstring const VALUE_KEY = L"edgeCaseStringKey";
			std::string const VALUE = "dfdf";
			std::string const SHORT_VALUE = "dfd";

			registryKey.setString(VALUE_KEY, VALUE);
			registryKey.setString(VALUE_KEY, SHORT_VALUE);
			REQUIRE(registryKey.getString(VALUE_KEY) == SHORT_VALUE);
		}
		SECTION("umlauts in string") {
			std::wstring const VALUE_KEY = L"edgeCaseString2";
			std::string const VALUE = u8"höllö";

			registryKey.setString(VALUE_KEY, VALUE);
			REQUIRE(registryKey.getString(VALUE_KEY) == VALUE);
		}
	}
	SECTION("existing RegistryKey") {
		RegistryKey registryKey(SUBKEY);

		SECTION("wasCreatedNewKey") {
			REQUIRE_FALSE(registryKey.wasCreatedNewKey());
		}
	}
}