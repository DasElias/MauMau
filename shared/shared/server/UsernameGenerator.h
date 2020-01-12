#pragma once
#include <vector>
#include <string>

namespace card {
	std::string generateUsername();
	std::string generateUsername(bool& out_isMale);
	std::string generateUsernameNotIn(const std::vector<std::string>& otherUsernames);
	std::string generateUsernameNotIn(const std::vector<std::string>& otherUsernames, bool& out_isMale);
}