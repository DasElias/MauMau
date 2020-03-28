#include "CardIndexTextures.h"
#include "CardIndexTextures.h"
#include "../utils/FileUtils.h"
#include "../renderingModel/SimpleTextureFactory.h"
#include <res/cardIndex/CardIndex.png.h>

namespace card {
	CardIndexTextures::CardIndexTextures() :
			textures({
				loadCardIndexTex(tex_cardIndex_heart, tex_cardIndex_heart_size),
				loadCardIndexTex(tex_cardIndex_spade, tex_cardIndex_spade_size),
				loadCardIndexTex(tex_cardIndex_diamond, tex_cardIndex_diamond_size),
				loadCardIndexTex(tex_cardIndex_club, tex_cardIndex_club_size)
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
	SimpleTexture CardIndexTextures::loadCardIndexTex(const unsigned char* imgData, std::size_t imgDataLength) {
		return SimpleTextureFactory()
			.setMagFilter(TextureMagFilter::LINEAR)
			.setMinFilter(TextureMinFilter::LINEAR)
			.loadFromMemory(imgData, imgDataLength);
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