#define NOMINMAX
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "TextureArray.h"
#include <stdexcept>
#include <algorithm>
#include <nanovg/stb_image.h>

namespace card {
	TextureArray::TextureArray(std::vector<std::string>& textures, int32_t width, int32_t height) :
			width(width),
			height(height) {

		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D_ARRAY, texId);


		// set texture parameters
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// GL_NEAREST
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// GL_LINEAR_MIPMAP_LINEAR


		// set anisotropic filtering
		if(glfwExtensionSupported("GL_ARB_texture_filter_anisotropic")) {
			GLfloat max;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max);
			max = std::min(max, ANISOTROPIC_FILTERING_AMOUNT);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, max);
		}



		// TODO use multisampling?
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, width, height, GLsizei(textures.size()), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		for(std::size_t counter = 0; counter < textures.size(); counter++) {
			int loadedImageWidth, loadedImageHeight;

			// load image 
			unsigned char* img = stbi_load(textures[counter].c_str(), &loadedImageWidth, &loadedImageHeight, 0, STBI_rgb_alpha);

			// couldn't load image
			if(img == nullptr) {
				throw std::runtime_error(("there is no image at "+textures[counter]).c_str());
			}

			// image doesn't have the correct dimensions
			if(loadedImageWidth != width || loadedImageHeight != height) {
				throw std::runtime_error("Image (" + textures[counter] + ") has the dimensions " + std::to_string(loadedImageWidth) + "/" + std::to_string(loadedImageHeight) + ", but this texture array can store only images with the dimensions " + std::to_string(width) + "/" + std::to_string(height));
			}

			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, counter, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, img);
			stbi_image_free(img);
		}


		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	}

	uint32_t TextureArray::getTexId() const {
		return texId;
	}

	int32_t TextureArray::getWidth() const {
		return width;
	}

	int32_t TextureArray::getHeight() const {
		return height;
	}

	void TextureArray::cleanUp() {
		glDeleteTextures(1, &texId);
	}

}
