#include "CardTextures.h"
#include <shared/model/Card.h>
#include "../utils/FileUtils.h"
#include "TextureArrayFactory.h"
#include "SamplerObjectFactory.h"
#include <res/cards/Cards.png.h>

namespace card {
	CardTextures::CardTextures() :
			textureArrayImpl(TextureArrayFactory()
				.setMinFilter(TextureMinFilter::LINEAR_MIPMAP_NEAREST)
				.setMagFilter(TextureMagFilter::LINEAR)
				.setWrapS(TextureWrap::CLAMP_TO_BORDER)
				.setWrapT(TextureWrap::CLAMP_TO_BORDER)
				.loadFromMemory({{
					card_0, card_1, card_2, card_3, card_4, card_5, card_6, card_7, card_8, card_9,
					card_10, card_11, card_12, card_13, card_14, card_15, card_16, card_17, card_18, card_19,
					card_20, card_21, card_22, card_23, card_24, card_25, card_26, card_27, card_28, card_29,
					card_30, card_31, card_32, card_33, card_34, card_35, card_36, card_37, card_38, card_39,
					card_40, card_41, card_42, card_43, card_44, card_45, card_46, card_47, card_48, card_49,
					card_50, card_51, card_52
				}},
				{{
					card_0_size, card_1_size, card_2_size, card_3_size, card_4_size, card_5_size, card_6_size, card_7_size, card_8_size, card_9_size,
					card_10_size, card_11_size, card_12_size, card_13_size, card_14_size, card_15_size, card_16_size, card_17_size, card_18_size, card_19_size,
					card_20_size, card_21_size, card_22_size, card_23_size, card_24_size, card_25_size, card_26_size, card_27_size, card_28_size, card_29_size,
					card_30_size, card_31_size, card_32_size, card_33_size, card_34_size, card_35_size, card_36_size, card_37_size, card_38_size, card_39_size,
					card_40_size, card_41_size, card_42_size, card_43_size, card_44_size, card_45_size, card_46_size, card_47_size, card_48_size, card_49_size,
					card_50_size, card_51_size, card_52_size
				}})
			),
			highAnisotropicFilteringSampler(SamplerObjectFactory()
				.setMinFilter(TextureMinFilter::LINEAR_MIPMAP_NEAREST)
				.setMagFilter(TextureMagFilter::LINEAR)
				.setWrapS(TextureWrap::CLAMP_TO_BORDER)
				.setWrapT(TextureWrap::CLAMP_TO_BORDER)
				.setAnisotropicFiltering(16)
				.generateSampler()
		) {

		const unsigned char* ptr = card_0;
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