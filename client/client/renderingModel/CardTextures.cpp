#include "CardTextures.h"
#include <shared/model/Card.h>
#include "../utils/FileUtils.h"
#include "SimpleTextureFactory.h"
#include "TextureArrayFactory.h"
#include "SamplerObjectFactory.h"
namespace card {
	std::vector<std::string> CardTextures::getTexturePaths() {
		std::vector<std::string> paths;
		std::string folder = "C:\\Users\\Elias\\Documents\\PROJECT X\\Appdata-Folder\\resources\\cards\\";

		for(int id = 0; id <= Card::MAX_CARDS; id++) {
			paths.push_back(folder + std::to_string(id) + ".png");
		}

		return paths;
	}
	CardTextures::CardTextures() :
		textureArrayImpl(SimpleTextureFactory()
						 .setMinFilter(TextureMinFilter::LINEAR_MIPMAP_NEAREST)
						 .setMagFilter(TextureMagFilter::LINEAR)
						 .setWrapS(TextureWrap::CLAMP_TO_BORDER)
						 .setWrapT(TextureWrap::CLAMP_TO_BORDER)
						 .loadFromFile("C:\\Users\\Elias\\Documents\\PROJECT X\\svgcards\\svg-cards2.png")
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