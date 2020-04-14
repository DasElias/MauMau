#pragma once
#include <Windows.h>


namespace card {
	struct RegistryHandleWrapper {
		HKEY registryHandle;

		RegistryHandleWrapper() : registryHandle(nullptr) {}
	};
}