#include "RegistryKey.h"
#include "RegistryException.h"
#include <Windows.h>
#include "RegistryHandleWrapper.h"

namespace card {
	RegistryKey::RegistryKey(std::wstring subKey) :
			registryHandleWrapper(std::make_unique<RegistryHandleWrapper>()) {
		DWORD disposition;
		if(LSTATUS ec = RegCreateKeyExW(HKEY_CURRENT_USER, subKey.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ, NULL, &registryHandleWrapper->registryHandle, &disposition) != ERROR_SUCCESS) {
			throw RegistryException("Could not open registry key.");
		}

		this->wasCreatedNewKey_field = disposition == REG_CREATED_NEW_KEY;
	}
	RegistryKey::~RegistryKey() {
		auto registryHandle = registryHandleWrapper->registryHandle;
		if(registryHandle != nullptr) RegCloseKey(registryHandle);
	}
	bool card::RegistryKey::wasCreatedNewKey() const {
		return wasCreatedNewKey_field;
	}
}