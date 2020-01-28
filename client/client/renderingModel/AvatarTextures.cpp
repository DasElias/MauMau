#include "AvatarTextures.h"
#include "AvatarTextures.h"
#include "AvatarTextures.h"
#include <shared/model/AvatarUtils.h>
#include "../utils/FileUtils.h"
#include "SimpleTextureFactory.h"
#include <stdexcept>
#include <boost/filesystem.hpp>
#include <res/avatars/Avatars.png.h>

namespace card {
	AvatarTextures::AvatarTextures() :
			avatarNotFoundTexture(SimpleTextureFactory().setMinFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR).loadFromMemory(avatar_0, avatar_0_size)),
			width(-1),
			height(-1) {

		initTextures();
	}
	void card::AvatarTextures::initTextures() {
		static std::vector<std::tuple<Avatar, const unsigned char*, std::size_t>> const avatarImages{{
			{1, avatar_1, avatar_1_size},
			{2, avatar_2, avatar_2_size},
			{3, avatar_3, avatar_3_size},
			{4, avatar_4, avatar_4_size},
			{5, avatar_5, avatar_5_size},
			{6, avatar_6, avatar_6_size},
			{7, avatar_7, avatar_7_size},
			{8, avatar_8, avatar_8_size},
			{9, avatar_9, avatar_9_size},
			{10, avatar_10, avatar_10_size},
			{11, avatar_11, avatar_11_size},
			{12, avatar_12, avatar_12_size},
			{13, avatar_13, avatar_13_size},
			{14, avatar_14, avatar_14_size},
			{15, avatar_15, avatar_15_size},
			{16, avatar_16, avatar_16_size},
			{17, avatar_17, avatar_17_size},
			{18, avatar_18, avatar_18_size},
			{19, avatar_19, avatar_19_size},
			{20, avatar_20, avatar_20_size},
			{21, avatar_21, avatar_21_size},
			{22, avatar_22, avatar_22_size},
		}};
		
		if(avatarImages.size() != getAllAvatars().size()) {
			throw std::runtime_error("Avatars couldn't be loaded correctly");
		}

		for(auto& avatarTuple : avatarImages) {
			Avatar avatar = std::get<0>(avatarTuple);
			const unsigned char* imgData = std::get<1>(avatarTuple);
			std::size_t imgDataSize = std::get<2>(avatarTuple);

			SimpleTexture tex = SimpleTextureFactory()
				.setMinFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR)
				.loadFromMemory(imgData, imgDataSize);

			std::int32_t localWidth = tex.getWidth();
			std::int32_t localHeight = tex.getHeight();
			if((width != -1 && localWidth != width) || (height != -1 && localHeight != height)) {
				throw std::runtime_error("Not all images have the same dimensions.");
			}

			this->width = localWidth;
			this->height = localHeight;

			textures.insert(std::pair<Avatar, SimpleTexture>(avatar, tex));
		}
	}
	void AvatarTextures::bind(Avatar avatar) const {
		if(textures.find(avatar) != textures.end()) textures.at(avatar).bind();
		else avatarNotFoundTexture.bind();
	}
	std::int32_t AvatarTextures::getWidth() const {
		return width;
	}
	std::int32_t AvatarTextures::getHeight() const {
		return height;
	}
	float AvatarTextures::getAspectRatio() const {
		return float(width)/height;
	}
	void AvatarTextures::cleanUp() {
		avatarNotFoundTexture.cleanUp();

		for(auto& avatar : this->textures) {
			SimpleTexture avatarTexture = avatar.second;
			avatarTexture.cleanUp();
		}
	}
}