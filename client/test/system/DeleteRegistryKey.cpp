#include "DeleteRegistryKey.h"
#include <catch2/catch.hpp>
#include <Windows.h>

namespace card {
	void deleteRegistryKey(std::wstring parentKey, std::wstring subKey) {
		HKEY key;
		if(RegCreateKeyExW(HKEY_CURRENT_USER, parentKey.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, NULL) != ERROR_SUCCESS) {
			FAIL();
		}
		LSTATUS ec = RegDeleteKeyW(key, subKey.c_str());
		if(ec != ERROR_SUCCESS && ec != ERROR_FILE_NOT_FOUND) {
			FAIL();
		}
		RegCloseKey(key);
	}
}