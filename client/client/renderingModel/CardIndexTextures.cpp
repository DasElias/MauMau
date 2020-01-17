#include "CardIndexTextures.h"
#include "CardIndexTextures.h"
#include "../utils/FileUtils.h"
#include "../renderingModel/SimpleTextureFactory.h"

namespace card {
	CardIndexTextures::CardIndexTextures() :
			textures({
				loadCardIndexTex("heart.png"),
				loadCardIndexTex("spade.png"),
				loadCardIndexTex("diamond.png"),
				loadCardIndexTex("club.png")
			}),
			aspectRatio(-1) {
		
		for(int i = 0; i < textures.size(); i++) {
			auto tex = textures[i];
			images[i] = egui::Image::loadFromOpenglTexture(tex.getTexId(), tex.getWidth(), tex.getHeight());
		}

		for(auto& t : images) {
			float texAspectRatio = t->getWidth() / float(t->getHeight());

			float const TOLERANCE = 0.01f;
			bool isAspectRatioNotEqual = (abs(this->aspectRatio - texAspectRatio) > TOLERANCE);
			if(this->aspectRatio != -1 && isAspectRatioNotEqual) {
				throw std::runtime_error("Aspect ratio of all card index textures must be the same");
			}
			this->aspectRatio = texAspectRatio;
		}
	}
	SimpleTexture CardIndexTextures::loadCardIndexTex(std::string texName) {
		std::string const path = getApplicationFolder() + "\\resources\\cards\\" + texName;
		return SimpleTextureFactory(path)
			.setMagFilter(TextureMagFilter::LINEAR)
			.setMinFilter(TextureMinFilter::LINEAR)
			.generateTexture();
	}
	std::shared_ptr<egui::Image> CardIndexTextures::getImage(CardIndex cardIndex) const {
		if(cardIndex == CardIndex::NULLINDEX) return nullptr;
		
		int index = static_cast<int>(cardIndex);
		index--;
		return images[index];
	}
	SimpleTexture CardIndexTextures::getTexture(CardIndex cardIndex) const {
		int index = static_cast<int>(cardIndex);
		index--;
		return textures[index];
	}
	float CardIndexTextures::getAspectRatio() const {
		return aspectRatio;
	}
}