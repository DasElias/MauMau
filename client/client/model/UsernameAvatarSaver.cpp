#include "UsernameAvatarSaver.h"
#include "../system/RegistryKey.h"

namespace card {
	static std::wstring const USERNAME_KEY = L"username";
	static std::wstring const AVATAR_KEY = L"avatar";
	static std::wstring const SUBKEY = L"Software\\MauMau 3D\\menu";

	UsernameAvatarSaver::UsernameAvatarData UsernameAvatarSaver::load() {
		RegistryKey registryKey(SUBKEY);
		std::string username = registryKey.getStringOrDefault(USERNAME_KEY, "");
		Avatar avatar = registryKey.getDwordOrDefault(AVATAR_KEY, 0);
		return std::make_pair(username, avatar);
	}
	void UsernameAvatarSaver::save(UsernameAvatarData data) {
		RegistryKey registryKey(SUBKEY);
		auto[username, avatar] = data;
		registryKey.setString(USERNAME_KEY, username);
		registryKey.setDword(AVATAR_KEY, avatar);
	}
}


