#include "DwordRegistryKey.h"
#include "RegistryException.h"
#include "RegistryHandleWrapper.h"

namespace card {
	static const DWORD VALUE_TYPE = REG_DWORD;

	DwordRegistryKey::DwordRegistryKey(std::wstring subKey) :
			RegistryKey(subKey) {
	}

	void DwordRegistryKey::setValue(std::wstring valueName, uint32_t value) {
		DWORD castedValue = static_cast<DWORD>(value);
		if(LSTATUS ec = RegSetValueExW(registryHandleWrapper->registryHandle, valueName.c_str(), 0, VALUE_TYPE, reinterpret_cast<LPBYTE>(&castedValue), sizeof(castedValue)) != ERROR_SUCCESS) {
			throw RegistryException("Could not set the value of the registry key.");
		}
	}

	uint32_t DwordRegistryKey::getValue(std::wstring valueName) {
		DWORD data = 0;
		DWORD dataSize;
		DWORD dataType = VALUE_TYPE;

		if(RegQueryValueExW(registryHandleWrapper->registryHandle, valueName.c_str(), 0, &dataType, reinterpret_cast<LPBYTE>(&data), &dataSize) != ERROR_SUCCESS) {
			throw RegistryException("Could not query the value of the registry key.");
		}
		
		return data;
	}
	uint32_t card::DwordRegistryKey::getValueOrDefault(std::wstring valueName, uint32_t defaultValue) {
		DWORD data = 0;
		DWORD dataSize;
		DWORD dataType = VALUE_TYPE;

		LSTATUS ec = RegQueryValueExW(registryHandleWrapper->registryHandle, valueName.c_str(), 0, &dataType, reinterpret_cast<LPBYTE>(&data), &dataSize);
		if(ec == ERROR_SUCCESS) {
			return data;
		} else if(ec == ERROR_FILE_NOT_FOUND) {
			return defaultValue;
		} else {
			throw RegistryException("Could not query the value of the registry key.");
		}
	}
}