#include "CardTextures.h"
#include <shared/model/Card.h>
#include "../utils/FileUtils.h"

namespace card {
	CardTextures::CardTextures() :
			textureArrayImpl(getTexturePaths(), CARD_TEXTURE_WIDTH, CARD_TEXTURE_HEIGHT) {
	}
	std::vector<std::string> CardTextures::getTexturePaths() {
		std::vector<std::string> paths;
		std::string folder = getApplicationFolder() + "\\resources\\cards\\";

		for(int id = 0; id <= Card::MAX_CARDS; id++) {
			paths.push_back(folder + std::to_string(id) + ".png");
		}

		return paths;
	}
	uint32_t CardTextures::getTexId() const {
		return textureArrayImpl.getTexId();
	}
	int32_t CardTextures::getWidth() const {
		return textureArrayImpl.getWidth();
	}
	int32_t CardTextures::getHeight() const {
		return textureArrayImpl.getHeight();
	}
	void CardTextures::cleanUp() {
		textureArrayImpl.cleanUp();
	}
}