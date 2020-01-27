#include "TextureArrayFactory.h"
#include "TexturePropertiesConverter.h"
#include <nanovg/stb_image.h>
#include <stdexcept>
#include <algorithm>

namespace card {
	TextureArrayFactory::TextureArrayFactory() :
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
	TextureArray TextureArrayFactory::loadFromFiles(std::vector<std::string> paths) {
		std::vector<TextureArrayData> texData;
		for(auto& p : paths) {
			int32_t width, height;
			unsigned char* stbiHandle = stbi_load(p.c_str(), &width, &height, 0, STBI_rgb_alpha);
		
			if(stbiHandle == nullptr) {
				throw std::runtime_error("there is no image at " + p);
			}

			texData.push_back({stbiHandle, width, height});
		}

		TextureArray texArray = load(texData);
		free(texData);
		return texArray;
	}
	TextureArray TextureArrayFactory::loadFromMemory(std::vector<const unsigned char*> imageData, std::vector<std::size_t> imageDataLengths) {
		if(imageData.size() != imageDataLengths.size()) {
			throw std::runtime_error("Size of imageData and imageDataLengths must be the same");
		}

		std::vector<TextureArrayData> texData;
		for(int i = 0; i < imageData.size(); i++) {
			const unsigned char* data = imageData[i];
			std::size_t dataLength = imageDataLengths[i];

			int32_t width, height;
			unsigned char* stbiHandle = stbi_load_from_memory(data, dataLength, &width, &height, 0, STBI_rgb_alpha);

			if(stbiHandle == nullptr) {
				throw std::runtime_error("Couldn't load image from memory!");
			}

			texData.push_back({stbiHandle, width, height});

		}
		
		TextureArray texArray = load(texData);
		free(texData);
		return texArray;
	}
	TextureArray TextureArrayFactory::load(const std::vector<TextureArrayData>& texData) {
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

		for(int counter = 0; counter < texData.size(); counter++) {
			auto data = texData[counter];
			int32_t loadedImageWidth = data.width;
			int32_t loadedImageHeight = data.height;

			if((width != -1 && loadedImageWidth != width) || (height != -1 && loadedImageHeight != height)) {
				throw std::runtime_error("Not all images have the same dimensions.");
			}
			width = loadedImageWidth;
			height = loadedImageHeight;
		}

		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, width, height, GLsizei(texData.size()), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		for(int i = 0; i < texData.size(); i++) {
			unsigned char* imgData = texData[i].imgData;

			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
		}

		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

		return TextureArray(texId, width, height);
	}
	void TextureArrayFactory::free(const std::vector<TextureArrayData>& texData) {
		for(auto& img : texData) {
			stbi_image_free(img.imgData);
		}
	}
}