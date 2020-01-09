#include "TextureArrayFactory.h"
#include "TexturePropertiesConverter.h"
#include <nanovg/stb_image.h>
#include <stdexcept>
#include <algorithm>

namespace card {
	TextureArrayFactory::TextureArrayFactory(std::vector<std::string> path) :
		textures(path),
		minFilter(TextureMinFilter::NEAREST_MIPMAP_LINEAR),
		magFilter(TextureMagFilter::LINEAR),
		wrapS(TextureWrap::REPEAT),
		wrapT(TextureWrap::REPEAT),
		anisotropicFiltering(1) {
	}
	TextureArrayFactory& TextureArrayFactory::setMinFilter(TextureMinFilter minFilter) {
		this->minFilter = minFilter;
		return *this;
	}
	TextureArrayFactory& TextureArrayFactory::setMagFilter(TextureMagFilter magFilter) {
		this->magFilter = magFilter;
		return *this;
	}
	TextureArrayFactory& TextureArrayFactory::setWrapS(TextureWrap wrap) {
		this->wrapS = wrap;
		return *this;
	}
	TextureArrayFactory& TextureArrayFactory::setWrapT(TextureWrap wrap) {
		this->wrapT = wrap;
		return *this;
	}
	TextureArrayFactory& TextureArrayFactory::setAnisotropicFiltering(float a) {
		this->anisotropicFiltering = a;
		return *this;
	}
	TextureArray TextureArrayFactory::generateTexture() {
		uint32_t texId;
		int32_t width = -1, height = -1;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D_ARRAY, texId);


		// set texture parameters
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, convertWrapValue(wrapS));
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, convertWrapValue(wrapT));
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, convertMagFilter(magFilter));	
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, convertMinFilter(minFilter));	


		// set anisotropic filtering
		if(glfwExtensionSupported("GL_ARB_texture_filter_anisotropic")) {
			GLfloat max;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max);
			max = std::min(max, anisotropicFiltering);

			glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_ANISOTROPY, max);
		}

		std::vector<unsigned char*> images;
		for(int counter = 0; counter < textures.size(); counter++) {
			std::string texturePath = textures[counter];
			
			int32_t loadedImageWidth, loadedImageHeight;
			unsigned char* i = stbi_load(texturePath.c_str(), &loadedImageWidth, &loadedImageHeight, 0, STBI_rgb_alpha);

			if(i == nullptr) {
				throw std::runtime_error("there is no image at " + texturePath);
			}
			if((width != -1 && loadedImageWidth != width) || (height != -1 && loadedImageHeight != height)) {
				throw std::runtime_error("Not all images have the same dimensions.");
			}
			width = loadedImageWidth;
			height = loadedImageHeight;

			images.push_back(i);
		}

		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, width, height, GLsizei(textures.size()), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		for(int i = 0; i < images.size(); i++) {
			auto imgData = images[i];

			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
			stbi_image_free(imgData);
		}

		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

		return TextureArray(texId, width, height);
	}
}