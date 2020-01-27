#include "SimpleTextureFactory.h"
#include "TexturePropertiesConverter.h"
#include <nanovg/stb_image.h>
#include <stdexcept>
#include <algorithm>

namespace card {
	SimpleTextureFactory::SimpleTextureFactory() :
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
	SimpleTexture SimpleTextureFactory::loadFromFile(std::string path) {
		int32_t width, height;
		unsigned char* stbiHandle = stbi_load(path.c_str(), &width, &height, 0, STBI_rgb_alpha);

		if(stbiHandle == nullptr) {
			throw std::runtime_error("there is no image at " + path);
		}

		SimpleTexture tex = load(stbiHandle, width, height);
		free(stbiHandle);
		return tex;
	}
	SimpleTexture SimpleTextureFactory::loadFromMemory(const unsigned char* imgData, std::size_t imgDataLength) {
		int32_t width, height;
		unsigned char* stbiHandle = stbi_load_from_memory(imgData, imgDataLength, &width, &height, 0, STBI_rgb_alpha);

		if(stbiHandle == nullptr) {
			throw std::runtime_error("Couldn't load image from memory!");
		}

		SimpleTexture tex = load(stbiHandle, width, height);
		free(stbiHandle);
		return tex;
	}
	SimpleTexture SimpleTextureFactory::load(unsigned char* stbiHandle, int32_t width, int32_t height) {
		uint32_t texId;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, convertWrapValue(wrapS));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, convertWrapValue(wrapT));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, convertMinFilter(minFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, convertMagFilter(magFilter));

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, stbiHandle);
		glGenerateMipmap(GL_TEXTURE_2D);

		// set anisotropic filtering
		if(glfwExtensionSupported("GL_ARB_texture_filter_anisotropic")) {
			GLfloat max;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max);
			max = std::min(max, anisotropicFiltering);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, max);
		}

		return SimpleTexture(texId, width, height);
	}
	void SimpleTextureFactory::free(unsigned char* stbiHandle) {
		stbi_image_free(stbiHandle);
	}
}