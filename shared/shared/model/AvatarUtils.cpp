#include "AvatarUtils.h"
#include <stdexcept>
#include "../utils/MathUtils.h"
#include <optional>

namespace card {
	std::vector<Avatar> const males = {
		1, 2, 3, 4, 6, 11, 14, 15, 16
	};
	std::vector<Avatar> const females = {
		5, 7, 8, 9, 10, 12, 13
	};

	Avatar getRandomMaleAvatar() {
		if(males.empty()) throw std::runtime_error("No avatar available");
		std::size_t index = randomInRange<std::size_t>(0, males.size() - 1);

		return males[index];
	}
	Avatar getRandomFemaleAvatar() {
		if(females.empty()) throw std::runtime_error("No avatar available");
		std::size_t index = randomInRange<std::size_t>(0, females.size() - 1);

		return females[index];
	}
	Avatar getRandomAvatar(bool isMale) {
		if(isMale) return getRandomMaleAvatar();
		else return getRandomFemaleAvatar();
	}
	std::vector<Avatar> getAllMaleAvatars() {
		return males;
	}
	std::vector<Avatar> getAllFemaleAvatars() {
		return females;
	}
	std::vector<Avatar> getAllAvatars() {
		static std::optional<std::vector<Avatar>> combined;

		if(! combined.has_value()) {
			combined = std::vector<Avatar>();
			combined->reserve(males.size() + females.size());
			combined->insert(combined->end(), males.begin(), males.end());
			combined->insert(combined->end(), females.begin(), females.end());
		}

		return *combined;
	}
}