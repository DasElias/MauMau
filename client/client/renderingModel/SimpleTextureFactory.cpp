#include "SimpleTextureFactory.h"
#include "TexturePropertiesConverter.h"
#include <nanovg/stb_image.h>
#include <stdexcept>
#include <algorithm>

namespace card {
	SimpleTextureFactory::SimpleTextureFactory(std::string path) :
			path(path),
			minFilter(TextureMinFilter::NEAREST_MIPMAP_LINEAR),
			magFilter(TextureMagFilter::LINEAR),
			wrapS(TextureWrap::REPEAT),
			wrapT(TextureWrap::REPEAT),
			anisotropicFiltering(1) {
	}
	SimpleTextureFactory& SimpleTextureFactory::setMinFilter(TextureMinFilter minFilter) {
		this->minFilter = minFilter;
		return *this;
	}
	SimpleTextureFactory& SimpleTextureFactory::setMagFilter(TextureMagFilter magFilter) {
		this->magFilter = magFilter;
		return *this;
	}
	SimpleTextureFactory& SimpleTextureFactory::setWrapS(TextureWrap wrap) {
		this->wrapS = wrap;
		return *this;
	}
	SimpleTextureFactory& SimpleTextureFactory::setWrapT(TextureWrap wrap) {
		this->wrapT = wrap;
		return *this;
	}
	SimpleTextureFactory& SimpleTextureFactory::setAnisotropicFiltering(float a) {
		this->anisotropicFiltering = a;
		return *this;
	}
	SimpleTexture SimpleTextureFactory::generateTexture() {
		int32_t width, height;
		unsigned char* img = stbi_load(path.c_str(), &width, &height, 0, STBI_rgb_alpha);
		if(!img) {
			throw std::runtime_error("Couldn't load image!");
		}

		uint32_t texId;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, convertWrapValue(wrapS));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, convertWrapValue(wrapT));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, convertMinFilter(minFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, convertMagFilter(magFilter));

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
		glGenerateMipmap(GL_TEXTURE_2D);

		// set anisotropic filtering
		if(glfwExtensionSupported("GL_ARB_texture_filter_anisotropic")) {
			GLfloat max;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max);
			max = std::min(max, anisotropicFiltering);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, max);
		}
		stbi_image_free(img);

		return SimpleTexture(texId, width, height);
	}
}