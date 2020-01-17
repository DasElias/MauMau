#include "AvatarTextures.h"
#include "AvatarTextures.h"
#include "AvatarTextures.h"
#include <shared/model/AvatarUtils.h>
#include "../utils/FileUtils.h"
#include "SimpleTextureFactory.h"
#include <stdexcept>
#include <boost/filesystem.hpp>

namespace card {
	AvatarTextures::AvatarTextures() :
			avatarNotFoundTexture(SimpleTextureFactory(getApplicationFolder() + "\\resources\\avatars\\notfound.png").setMinFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR).generateTexture()),
			width(-1),
			height(-1) {

		initTextures();
	}
	void card::AvatarTextures::initTextures() {
		std::string const folder = getApplicationFolder() + "\\resources\\avatars\\";
		auto const allAvatars = getAllAvatars();

		for(auto& avatar : allAvatars) {
			std::string path = folder + std::to_string(avatar) + ".png";
			if(! boost::filesystem::exists(path)) continue;

			SimpleTexture tex = SimpleTextureFactory(path)
				.setMinFilter(TextureMinFilter::LINEAR_MIPMAP_LINEAR)
				.generateTexture();

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