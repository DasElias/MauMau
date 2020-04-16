#pragma once
#include <utility>
#include <shared/model/Avatar.h>
#include <string>

namespace card {
	namespace UsernameAvatarSaver {
		typedef std::pair<std::string, Avatar> UsernameAvatarData;

		UsernameAvatarData load();
		void save(UsernameAvatarData data);
	}
}