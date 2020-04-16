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
	void RegistryKey::setDword(std::wstring valueName, uint32_t value) {
		DWORD castedValue = static_cast<DWORD>(value);
		if(LSTATUS ec = RegSetValueExW(registryHandleWrapper->registryHandle, valueName.c_str(), 0, REG_DWORD, reinterpret_cast<LPBYTE>(&castedValue), sizeof(castedValue)) != ERROR_SUCCESS) {
			throw RegistryException("Could not set the value of the registry key.");
		}
	}
	uint32_t RegistryKey::getDword(std::wstring valueName) {
		DWORD data = 0;
		DWORD dataSize = sizeof(DWORD);

		HKEY registryHandle = registryHandleWrapper->registryHandle;
		if(RegQueryValueExW(registryHandle, valueName.c_str(), 0, NULL, reinterpret_cast<LPBYTE>(&data), &dataSize) != ERROR_SUCCESS) {
			throw RegistryException("Could not query the value of the registry key.");
		}

		return data;
	}
	uint32_t RegistryKey::getDwordOrDefault(std::wstring valueName, uint32_t defaultValue) {
		DWORD data = 0;
		DWORD dataSize = sizeof(DWORD);

		HKEY registryHandle = registryHandleWrapper->registryHandle;
		LSTATUS ec = RegQueryValueExW(registryHandle, valueName.c_str(), 0, NULL, reinterpret_cast<LPBYTE>(&data), &dataSize);
		if(ec == ERROR_SUCCESS) {
			return data;
		} else if(ec == ERROR_FILE_NOT_FOUND) {
			return defaultValue;
		} else {
			throw RegistryException("Could not query the value of the registry key.");
		}
	}
	void RegistryKey::setString(std::wstring valueName, std::string value) {
		const char* castedValue = value.c_str();
		if(LSTATUS ec = RegSetValueExW(registryHandleWrapper->registryHandle, valueName.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE*>(castedValue), value.size() + 1) != ERROR_SUCCESS) {
			throw RegistryException("Could not set the value of the registry key.");
		}
	}
	std::string RegistryKey::getString(std::wstring valueName) {
		char buffer[1024];
		DWORD size = sizeof(buffer);
		DWORD type = REG_SZ;

		HKEY registryHandle = registryHandleWrapper->registryHandle;
		if(RegQueryValueExW(registryHandle, valueName.c_str(), 0, &type, reinterpret_cast<LPBYTE>(&buffer), &size) != ERROR_SUCCESS) {
			throw RegistryException("Could not query the value of the registry key.");
		}

		return std::string(buffer);
	}
	std::string RegistryKey::getStringOrDefault(std::wstring valueName, std::string defaultValue) {
		char buffer[1024];
		DWORD size = sizeof(buffer);
		DWORD type = REG_SZ;

		HKEY registryHandle = registryHandleWrapper->registryHandle;
		LSTATUS ec = RegQueryValueExW(registryHandle, valueName.c_str(), 0, &type, reinterpret_cast<LPBYTE>(&buffer), &size);
		if(ec == ERROR_SUCCESS) {
			return std::string(buffer);
		} else if(ec == ERROR_FILE_NOT_FOUND) {
			return defaultValue;
		} else {
			throw RegistryException("Could not query the value of the registry key.");
		}
	}
}