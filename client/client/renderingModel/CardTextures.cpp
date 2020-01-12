#include "CardTextures.h"
#include <shared/model/Card.h>
#include "../utils/FileUtils.h"
#include "TextureArrayFactory.h"
#include "SamplerObjectFactory.h"
namespace card {
	CardTextures::CardTextures() :
			textureArrayImpl(TextureArrayFactory(getTexturePaths())
				.setMinFilter(TextureMinFilter::LINEAR_MIPMAP_NEAREST)
				.setMagFilter(TextureMagFilter::LINEAR)
				.setWrapS(TextureWrap::CLAMP_TO_BORDER)
				.setWrapT(TextureWrap::CLAMP_TO_BORDER)
				.generateTexture()
			),
			highAnisotropicFilteringSampler(SamplerObjectFactory()
				.setMinFilter(TextureMinFilter::LINEAR_MIPMAP_NEAREST)
				.setMagFilter(TextureMagFilter::LINEAR)
				.setWrapS(TextureWrap::CLAMP_TO_BORDER)
				.setWrapT(TextureWrap::CLAMP_TO_BORDER)
				.setAnisotropicFiltering(16)
				.generateSampler()
		) {
	}
	std::vector<std::string> CardTextures::getTexturePaths() {
		std::vector<std::string> paths;
		std::string folder = getApplicationFolder() + "\\resources\\cards\\";

		for(int id = 0; id <= Card::MAX_CARDS; id++) {
			paths.push_back(folder + std::to_string(id) + ".png");
		}

		return paths;
	}
	void CardTextures::bindDefault() const {
		textureArrayImpl.bind();
	}
	void CardTextures::bindWithHighAnisotropy() const {
		bindDefault();
		highAnisotropicFilteringSampler.bind();
	}
	void CardTextures::unbind() const {
		highAnisotropicFilteringSampler.unbind();
	}
	void CardTextures::cleanUp() {
		textureArrayImpl.cleanUp();
		highAnisotropicFilteringSampler.cleanUp();
	}
}