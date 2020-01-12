#pragma once
#include "Avatar.h"

namespace card {
	Avatar getRandomMaleAvatar();
	Avatar getRandomFemaleAvatar();
	Avatar getRandomAvatar(bool isMale);
	std::vector<Avatar> getAllMaleAvatars();
	std::vector<Avatar> getAllFemaleAvatars();
	std::vector<Avatar> getAllAvatars();
}