#pragma once
#include <vector>
#include <string>

namespace card {
	std::string generateUsername();
	std::string generateUsernameNotIn(const std::vector<std::string>& otherUsernames);
}